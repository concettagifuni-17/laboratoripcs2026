#pragma once
#include <iostream>
#include <algorithm>
#include <set> // insieme ordinato senza duplicati: utile per salvare archi e vicini
#include <map> // associa una chiave al valore 
#include <list> // utile per mantenere l'ordine in cui gli archi sono stati aggiunti

// implemento la classe unidirected_edge per rappresentare un arco, il grafo è non diretto quindi l'arco (1 3) è uguale a (3 1)
template<typename T>
class undirected_edge {
    T nodo1_;
    T nodo2_;
public:
    //costruttore user-defined
    undirected_edge(const T& pnodo1, const T& pnodo2)
    {
        nodo1_ = std::min(pnodo1, pnodo2);
        nodo2_ = std::max(pnodo1, pnodo2);
    }
    //restituisco i valori
    T nodo1() const { return nodo1_;}
    T nodo2() const { return nodo2_;}

    //Metodi 
    T from() const { return nodo1_;}
    T to() const { return nodo2_;}

    //operatore di ordinamento
    bool operator<(const undirected_edge<T>& other) const {
        if (nodo1_ < other.nodo1_) {
            return true;
        }
        if (nodo1_ > other.nodo1_) {
            return false;
        }
        if (nodo2_ < other.nodo2_) {
            return true;
        }
        return false;
    }
    //operatore di uguaglianza
    bool operator==(const undirected_edge<T>& other) const {
        if (nodo1_ == other.nodo1_ ) {
            if (nodo2_ == other.nodo2_) {
                return true;
            }
        }
        return false;
    }
};

//opertore di stampa
template<typename T> 
std::ostream&
operator<<(std::ostream& os, const undirected_edge<T>& e) {
    os << "(" << e.from() << "," << e.to() << ")";
    return os;
}

//classe dei grafi
template<typename T>
class undirected_graph {
    std::map<T, std::set<T>> vicini;
    std::set<undirected_edge<T>> archi;
    std::list<undirected_edge<T>> ordine_archi;
    std::map<undirected_edge<T>, int> numeri_archi;

public:
    //costruttore di default 
    undirected_graph()
    {}

    //costruttore di copia
    undirected_graph(const undirected_graph<T>& other)
        : vicini(other.vicini),
          archi(other.archi),
          ordine_archi(other.ordine_archi),
          numeri_archi(other.numeri_archi)
    {}
    //nodi vicini
    std::set<T> neighbours(const T& nodo) const {
        auto pos = vicini.find(nodo);
        if (pos == vicini.end()) {
            return {};
        }
        return pos -> second; 
    }

    //aggiungere un arco al grafo
    void add_edge(const undirected_edge<T>& arco) {
        auto ret = archi.insert(arco);
        if (!ret.second) {
            return;
        }
        T nodo1 = arco.from();
        T nodo2 = arco.to();

        vicini[nodo1].insert(nodo2);
        vicini[nodo2].insert(nodo1);

        int numero = static_cast<int>(ordine_archi.size());

        ordine_archi.push_back(arco);
        numeri_archi[arco] = numero;
    }

    //restituire tutti gli archi
    std::set<undirected_edge<T>> all_edges() const {
        return archi;
    }
    //restituire tutti i nodi
    std::set<T> all_nodes() const {
        std::set<T> nodi;
        for (const auto& elemento : vicini) {
            nodi.insert(elemento.first);
        }
        return nodi;
    }
    //restituire il numero (posizione) associato a un arco
    int edge_number(const undirected_edge<T>& arco) const {
        auto pos = numeri_archi.find(arco);
        
        if (pos == numeri_archi.end()) {
            return -1;
        }
        return pos -> second;
    }
    //restituire l'arco associato a un numero (di posizione)
    undirected_edge<T> edge_at(int numero) const {
        int contatore = 0;
        for (const auto& arco : ordine_archi) {
            if (contatore == numero) {
                return arco;
            }
            contatore = contatore + 1;
        }
        return ordine_archi.front();
    }
    //differenza tra grafi
    undirected_graph<T> operator-(const undirected_graph<T>& other) const{
        undirected_graph<T> risultato;
        for (const auto& arco : archi) {
            if (other.archi.find(arco) == other.archi.end()) {
                risultato.add_edge(arco);
            }
        }
        return risultato;
    }
};