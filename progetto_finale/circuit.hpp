#pragma once
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <Eigen/Dense>

#include "graph.hpp"
#include "cicli_fondamentali.hpp"
#include "depina.hpp"
#include "grad_coniugato.hpp"

//rappresentiamo un arco tramite la coppia dei sue nodi
//il grafo è non orientato quindi l'arco (1,2) e (2,1) devono essere considerati uguali 
using ArcoNodi = std::pair<int,int>;
//rappresentiamo una maglia come ciclo ordinato di nodi
using CicloNodi = std::vector<int>;

enum class Tcomponente {Resistore, Generatore}; //creiamo il Tipo del componente letto dalla netlist
enum class MetodoCicli {Dfs, DePina};           //Metodi usati per trovare le maglie indipendenti del circuito

//struct che rappresenta ogni componente della netlist
struct Componenti {
    std::string nome; 
    Tcomponente type = Tcomponente::Resistore;   
    double valore = 0.0; 
    int node1 = 0; 
    int node2 = 0;
};
//il tipo 'di default' è il Resistore perchè nella netlist i resistori sono i componenti più comuni

//risultato calcolato su un resistore
struct RisultatiR { 
    std::string nome; 
    double tensione = 0.0; 
    double correnti = 0.0; 
};

//struct principale del circuito 
struct Circuiti {
    bool valid = true;
    std::vector<Componenti> componenti, resistori, generatori;
    std::map<ArcoNodi,int> resistore_su_arco, generatore_su_arco; //due mappe che collegano ogni arco al resistore o al generatore presente su quell'arco
    undirected_graph<int> grafo;
};

struct SoluzioneMaglie { 
    bool valid = true; 
    std::vector<RisultatiR> risultati; //ogni elemento del vettore è un RisultatiR cioè la struct sopra 
};

// Funzione di utilità per stampare un messaggio di errore in modo uniforme.
// Restituisce false per poter essere usata facilmente anche nei controlli.
inline bool errore(const std::string& s) {
    std::cerr << "Errore: " << s << "\n"; 
    return false; 
}

// Restituisce una chiave canonica per un arco non orientato.
// Poiché il grafo è non orientato, gli archi (a,b) e (b,a) devono essere uguali.
// Per questo salviamo sempre la coppia con il nodo più piccolo per primo.
inline ArcoNodi chiave_arco(int a, int b) { 
    if (a < b) {
        return ArcoNodi{a, b};        
    }else {
        return ArcoNodi{b, a};
    }
}

// Definisce il verso convenzionale positivo della corrente su un arco.
// Scegliamo come verso positivo quello dal nodo più piccolo al nodo più grande.
// Se durante il calcolo la corrente viene negativa, significa che la corrente reale scorre nel verso opposto.
inline int verso_arco(int from, int to) { 
    if (from < to) {
        return 1;
    } else {
        return -1;
    }
}

// Confronta due numeri reali tenendo conto degli errori numerici.
// Non confrontiamo direttamente a == b perché i calcoli in virgola mobile possono produrre piccole differenze dovute all'approssimazione.
inline bool vicino(double a, double b) {
    double scala = 1.0 + std::max(std::abs(a), std::abs(b));
    return std::abs(a - b) <= 1.0e-7 * scala;
}

//funzioni per irrobustire il codice
inline std::string togli_commento(const std::string& riga) {
    std::size_t p1 = riga.find('#'), p2 = riga.find("//"), p = std::string::npos;
    if (p1 != std::string::npos) p = p1;
    if (p2 != std::string::npos) {
        if (p == std::string::npos || p2 < p) p = p2;
    }
    if (p == std::string::npos) {
        return riga;
    }else {
        return riga.substr(0, p);
    }
}
inline bool riga_vuota(const std::string& riga) {
    for (char c : riga) {
        if (!std::isspace(static_cast<unsigned char>(c))){
        return false;
        }
    }
    return true;
}

// Legge i componenti dalla netlis, ogni riga valida deve avere il formato: nome valore nodo1 nodo2
// Durante la lettura vengono eseguiti controlli di robustezza:
// - formato corretto della riga
// - tipo di componente valido, cioè R/r oppure V/v
// - nomi duplicati (ogni componenete deve essere identificato in modo univoco nel circuito)
// - se ci sono componenti collegate allo stesso nodo
// - se la resistenza positiva
inline std::vector<Componenti> read_netlist(std::istream& file) {
    std::vector<Componenti> componenti; 
    std::set<std::string> nomi; 
    std::string riga; 
    int nriga = 0;
    while (std::getline(file, riga)) {
        nriga++; 
        riga = togli_commento(riga); 
        if (riga_vuota(riga)) continue;
        Componenti c; 
        std::string extra; 
        std::istringstream iss(riga);
        if (!(iss >> c.nome >> c.valore >> c.node1 >> c.node2) || (iss >> extra)) {
            errore("riga " + std::to_string(nriga) + " non valida"); 
            return {};
        }
        char iniziale = c.nome.empty() ? '\0' : c.nome[0];
        bool resistore = (iniziale == 'R' || iniziale == 'r');
        bool generatore = (iniziale == 'V' || iniziale == 'v');
        if (!resistore && !generatore) {
            errore("componente " + c.nome + " non valido");
            return {};
        }
        if (nomi.find(c.nome) != nomi.end()) {
            errore("nome duplicato: " + c.nome);
            return {};
        }
        if (c.node1 == c.node2) {
            errore("componente " + c.nome + " collegato allo stesso nodo");
            return {};
        }
        if (resistore) {
            c.type = Tcomponente::Resistore;
        } else {
            c.type = Tcomponente::Generatore;
        }
        if (c.type == Tcomponente::Resistore && c.valore <= 0.0) {
            errore("resistenza non positiva: " + c.nome);
            return {};
        }
        nomi.insert(c.nome);
        componenti.push_back(c);
    }
    if (componenti.empty()) errore("netlist vuota");
    return componenti;
}
// Provo a leggere esattamente quattro campi
// Se mancano dati oppure se c'è qualcosa in più nella riga, la netlist viene considerata non valida

//se il file non può essere aperto restituisce una lista vuota
inline std::vector<Componenti> read_netlist_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) { 
        errore("impossibile aprire " + filename); 
        return {}; 
    }
    return read_netlist(file);
}

// Funzione usata per ordinare i componenti.
// Prima ordina in base all'arco su cui si trova il componente, poi a parità di arco ordina per nome
inline bool component_less(const Componenti& a, const Componenti& b) {
    ArcoNodi aa = chiave_arco(a.node1, a.node2), ab = chiave_arco(b.node1, b.node2);
    if (aa == ab) { 
        return a.nome < b.nome;
    }
    return aa < ab;
}

// Controlla se il grafo del circuito è connesso
// Usiamo una visita DFS iterativa: partiamo da un nodo qualsiasi, visitiamo tutti i nodi raggiungibili e alla fine controlliamo
// se il numero di nodi visitati coincide con il numero totale di nodi.
// Un circuito non connesso non può essere risolto come un unico circuito
inline bool connected_graph(const undirected_graph<int>& g) {
    std::set<int> nodi = g.all_nodes();
    if (nodi.empty()) return false;
    std::set<int> visitati;
    std::vector<int> stack;
    int nodo_iniziale = 0;
    for (const int& nodo : nodi) {
        nodo_iniziale = nodo;
        break;
    }
    stack.push_back(nodo_iniziale);
    visitati.insert(nodo_iniziale);
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (const int& vicino : g.neighbours(u)) {
            if (visitati.find(vicino) == visitati.end()) {
                visitati.insert(vicino);
                stack.push_back(vicino);
            }
        }
    }
    return visitati.size() == nodi.size();
}

// Costruisce la struttura Circuiti a partire dai componenti letti dalla netlist:
// - ordiniamo i componenti
// - controlliamo che non ci siano componenti in parallelo sullo stesso arco
// - costruiamo il grafo del circuito
// - separiamo resistori e generatori
// - creiamo le mappe arco -> componente
// - controlliamo che il circuito sia risolvibile
// Il numero di maglie indipendenti viene calcolato con la formula: numero_cicli = numero_archi - numero_nodi + 1   (valida per grafi connessi)
inline Circuiti build_circuit(const std::vector<Componenti>& input) {
    Circuiti c;
    c.componenti = input;
    std::sort(c.componenti.begin(), c.componenti.end(), component_less);
    std::set<ArcoNodi> archi_usati;
    for (const Componenti& componente : c.componenti) {
        ArcoNodi arco = chiave_arco(componente.node1, componente.node2);
        if (archi_usati.find(arco) != archi_usati.end()) {
            c.valid = false;
            errore("componenti in parallelo");  //se un arco è già stato usato vuol dire chei componenti sono in parallelo
            return c;
        }
        archi_usati.insert(arco);
        c.grafo.add_edge(undirected_edge<int>(arco.first, arco.second));
        if (componente.type == Tcomponente::Resistore) {
            c.resistore_su_arco[arco] = static_cast<int>(c.resistori.size());
            c.resistori.push_back(componente);
        } else {
            c.generatore_su_arco[arco] = static_cast<int>(c.generatori.size());
            c.generatori.push_back(componente);
        }
        // memorizziamo in quale arco si trova ciascun componente, così durante il calcolo delle maglie possiamo recuperarlo velocemente.
    }
    int numero_archi = static_cast<int>(c.grafo.all_edges().size());
    int numero_nodi = static_cast<int>(c.grafo.all_nodes().size());
    int numero_cicli = numero_archi - numero_nodi + 1;
    if (c.resistori.empty()) {
        c.valid = false;
        errore("nessun resistore");
    }
    if (!connected_graph(c.grafo)) {
        c.valid = false;
        errore("grafo non connesso");
    }
    if (numero_cicli <= 0) {
        c.valid = false;
        errore("nessuna maglia indipendente");
    }
    return c;
}

// Converte un ciclo rappresentato come vettore di incidenza degli archi in un ciclo fatto da una sequenza ordinata di nodi.
// De Pina restituisce i cicli indicando quali archi appartengono al ciclo.
// Per usarli nel metodo delle maglie, però, ci serve percorrere il ciclo come lista ordinata di nodi.
// La funzione controlla anche che gli archi selezionati formino davvero un ciclo semplice: in un ciclo ogni nodo deve avere esattamente due vicini
inline CicloNodi incidence_to_cycle(const undirected_graph<int>& g, const BoolVector<int>& incidenza) {
    std::map<int,std::vector<int>> adiacenza_ciclo; 
    int n_archi_ciclo = 0;
    for (int i = 0; i < static_cast<int>(incidenza.size()); i++) 
    if (incidenza[i] != 0) {
        undirected_edge<int> arco = g.edge_at(i); 
        adiacenza_ciclo[arco.from()].push_back(arco.to()); 
        adiacenza_ciclo[arco.to()].push_back(arco.from()); 
        n_archi_ciclo++;
    }
    if (n_archi_ciclo == 0) return {};
    for (const auto& elemento : adiacenza_ciclo) {
        if (elemento.second.size() != 2) return {};
    }
    int nodo_iniziale = adiacenza_ciclo.begin()->first;
    int nodo_precedente = nodo_iniziale;
    int nodo_corrente = nodo_iniziale;
    int nodo_successivo = adiacenza_ciclo[nodo_iniziale][0];

    CicloNodi ciclo;
    ciclo.push_back(nodo_iniziale);
    for (int passo = 0; passo <= n_archi_ciclo; passo++) {
        ciclo.push_back(nodo_successivo);
        nodo_precedente = nodo_corrente;
        nodo_corrente = nodo_successivo;
        if (nodo_corrente == nodo_iniziale) {
            if (static_cast<int>(ciclo.size()) == n_archi_ciclo + 1) {
                return ciclo;
            } else {
                return {};
            }
        }
        const std::vector<int>& vicini = adiacenza_ciclo[nodo_corrente];
        if (vicini[0] == nodo_precedente) {
            nodo_successivo = vicini[1];
        } else {
            nodo_successivo = vicini[0];
        }
    }
    return {};
}

// Restituisce le maglie indipendenti del circuito usando il metodo scelto
// Se il metodo è DFS, usiamo i cicli fondamentali ottenuti dalla visita
// Se il metodo è De Pina, otteniamo prima i cicli come vettori di incidenza
// e poi li convertiamo in sequenze di nodi
// Alla fine controlliamo che: il numero di cicli trovati sia quello atteso, ogni ciclo sia chiuso, cioè inizi e finisca nello stesso nodo
inline std::vector<CicloNodi> get_cycles(const Circuiti& circuito, MetodoCicli metodo) {
    int numero_archi = static_cast<int>(circuito.grafo.all_edges().size());
    int numero_nodi = static_cast<int>(circuito.grafo.all_nodes().size());

    int numero_cicli_attesi = numero_archi - numero_nodi + 1;

    std::vector<CicloNodi> cicli;
    int sorgente = 0;
    for (const int& nodo : circuito.grafo.all_nodes()) {
        sorgente = nodo;
        break;
    }
    if (metodo == MetodoCicli::Dfs) {
        cicli = fundamental_cycles_dfs(circuito.grafo, sorgente);
    } else {
        std::vector<BoolVector<int>> cicli_incidenza = depina_minimum_cycle_basis(circuito.grafo, sorgente);
        for (const BoolVector<int>& incidenza : cicli_incidenza) {
            CicloNodi ciclo = incidence_to_cycle(circuito.grafo, incidenza);
            if (ciclo.empty()) {
                return {};
            }
            cicli.push_back(ciclo);
        }
    }
    if (static_cast<int>(cicli.size()) != numero_cicli_attesi) {
        return {};
    }
    for (const CicloNodi& ciclo : cicli) {
        if (ciclo.size() < 2) {
            return {};
        }
        if (ciclo.front() != ciclo.back()) {
            return {};
        }
    }
    return cicli;
}

//calcola il contributo del genratore 
inline double contributo_generatore(const Componenti& g, int from, int to, bool& ok) {
    ok = true;
    if (from == g.node2 && to == g.node1) {
        return g.valore;
    }
    if (from == g.node1 && to == g.node2) {
        return -g.valore;
    }
    ok = false;
    return 0.0;
}

//le incognite del sistema sono le correnti di maglia, una volta trovate applichiamo la legge di ohm
inline SoluzioneMaglie solve_method(const Circuiti& circuito, MetodoCicli metodo) {
    SoluzioneMaglie soluzione;
    if (!circuito.valid) {
        soluzione.valid = false;
        return soluzione;
    }
    std::vector<CicloNodi> cicli = get_cycles(circuito, metodo);
    if (cicli.empty()) {
        soluzione.valid = false;
        errore("cicli non trovati");
        return soluzione;
    }
    int numero_resistori = static_cast<int>(circuito.resistori.size());
    int numero_cicli = static_cast<int>(cicli.size());

    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(numero_resistori, numero_cicli);   //collega i resistori alle maglie, righe = R colonne = maglie
    Eigen::VectorXd b = Eigen::VectorXd::Zero(numero_cicli);                     //contiene i contributi dei generatori nelle condizioni di maglia
    for (int j = 0; j < numero_cicli; j++) {
        const CicloNodi& ciclo = cicli[j];
        for (int k = 0; k + 1 < static_cast<int>(ciclo.size()); k++) {
            int nodo_partenza = ciclo[k];
            int nodo_arrivo = ciclo[k + 1];
            ArcoNodi arco = chiave_arco(nodo_partenza, nodo_arrivo);
            bool arco_resistore = circuito.resistore_su_arco.count(arco) > 0;
            bool arco_generatore = circuito.generatore_su_arco.count(arco) > 0;
            if (arco_resistore) {
                int indice_resistore = circuito.resistore_su_arco.at(arco);
                B(indice_resistore, j) += verso_arco(nodo_partenza, nodo_arrivo);
            } else if (arco_generatore) {
                int indice_generatore = circuito.generatore_su_arco.at(arco);
                bool ok = true;
                b(j) += contributo_generatore(circuito.generatori[indice_generatore],nodo_partenza,nodo_arrivo,ok);
                if (!ok) {
                    soluzione.valid = false;
                    return soluzione;
                }
            }
        }
    }
    // risolviamo A* correnti_maglia = b con il grad_coniugato
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(numero_cicli, numero_cicli);
    for (int i = 0; i < numero_cicli; i++) {
        for (int j = 0; j < numero_cicli; j++) {
            for (int r = 0; r < numero_resistori; r++) {
                A(i, j) += B(r, i) * circuito.resistori[r].valore * B(r, j);
            }
        }
    }
    unsigned int iterazioni = 0;
    bool converged = false;
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(numero_cicli);
    Eigen::VectorXd correnti_maglia = conjugate_gradient(A,b,x0,1.0e-12,10000,iterazioni, converged);
    if (!converged) {
        soluzione.valid = false;
        errore("Gradiente coniugato non convergente");
        return soluzione;
    }
    Eigen::VectorXd residuo = b - A * correnti_maglia;
    double scala_residuo = 1.0 + b.norm();
    if (residuo.norm() > 1.0e-7 * scala_residuo) {
        soluzione.valid = false; 
        errore("gradiente coniugato non convergente");
        return soluzione;
    }
    for (int r = 0; r < numero_resistori; r++) {
        double corrente_resistore = 0.0;
        for (int j = 0; j < numero_cicli; j++) {
            corrente_resistore += B(r, j) * correnti_maglia(j);
        }
        double tensione_resistore = circuito.resistori[r].valore * corrente_resistore;
        RisultatiR risultato;
        risultato.nome = circuito.resistori[r].nome;
        risultato.tensione = tensione_resistore;
        risultato.correnti = corrente_resistore;
        soluzione.risultati.push_back(risultato);
    }
    return soluzione;
}

//risolve il circuito con entrambi i metodi, e se i risultati coincidono sotto una certa toelleranza il circuito è valido
inline std::vector<RisultatiR> solve_circuit_checked(const Circuiti& circuito) {
    SoluzioneMaglie soluzione_dfs = solve_method(circuito, MetodoCicli::Dfs);
    SoluzioneMaglie soluzione_depina = solve_method(circuito, MetodoCicli::DePina);
    if (!soluzione_dfs.valid || !soluzione_depina.valid) {
        errore("uno dei due metodi non ha prodotto una soluzione valida");
        return {};
    }
    if (soluzione_dfs.risultati.size() != soluzione_depina.risultati.size()) {
        errore("DFS e De Pina hanno prodotto un numero diverso di risultati");
        return {};
    }
    for (int i = 0; i < static_cast<int>(soluzione_dfs.risultati.size()); i++) {
        if (soluzione_dfs.risultati[i].nome != soluzione_depina.risultati[i].nome) {
            errore("DFS e De Pina non coincidono sul nome del resistore");
            return {};
        }
        if (!vicino(soluzione_dfs.risultati[i].tensione, soluzione_depina.risultati[i].tensione)) {
            errore("DFS e De Pina non coincidono sulla tensione di " + soluzione_dfs.risultati[i].nome);
            return {};
        }
        if (!vicino(soluzione_dfs.risultati[i].correnti, soluzione_depina.risultati[i].correnti)) {
            errore("DFS e De Pina non coincidono sulla corrente di " + soluzione_dfs.risultati[i].nome);
            return {};
        }
    }
    return soluzione_depina.risultati;
}

inline std::string format_number(double x) {
    if (std::abs(x) < 1.0e-10) {
        x = 0.0;
    }
    if (std::abs(x - std::round(x)) < 1.0e-9) {
        return std::to_string(static_cast<long long>(std::llround(x))); 
    }
    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << x;
    std::string s = out.str(); 
    while (!s.empty() && s.back() == '0') {
        s.pop_back();
    }
    if (!s.empty() && s.back() == '.') {
        s.pop_back();
    } 
    if (s == "-0") {
        return "0";
    }
    return s;
}

//stampa i riultati inali, per ogni resistore mostriamo tensione e corrente calcolate
inline void print_resistor_results(std::ostream& out, const std::vector<RisultatiR>& risultati) {
    for (const RisultatiR& r : risultati) {
        out << r.nome << ": V = " << format_number(r.tensione) << " volts, I = " << format_number(r.correnti) << " amps.\n";
    }
}