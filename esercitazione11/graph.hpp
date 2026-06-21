#pragma once

#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <list>


// ============================================================
// CLASSE unidirected_edge
//
// Rappresenta un arco non diretto.
// Quindi l'arco (1,3) è uguale all'arco (3,1).
// ============================================================

template<typename T>
class unidirected_edge {
private:
    T nodo1_;
    T nodo2_;

public:
    // Costruttore
    unidirected_edge(const T& pnodo1, const T& pnodo2)
    {
        nodo1_ = std::min(pnodo1, pnodo2);
        nodo2_ = std::max(pnodo1, pnodo2);
    }

    // Restituiscono i due nodi dell'arco
    T nodo1() const {
        return nodo1_;
    }

    T nodo2() const {
        return nodo2_;
    }

    // Stessa cosa, ma con nomi più comodi
    T from() const {
        return nodo1_;
    }

    T to() const {
        return nodo2_;
    }

    // Operatore di ordinamento.
    // Serve perché gli archi stanno dentro std::set e std::map.
    bool operator<(const unidirected_edge<T>& other) const {
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

    // Operatore di uguaglianza
    bool operator==(const unidirected_edge<T>& other) const {
        if (nodo1_ == other.nodo1_) {
            if (nodo2_ == other.nodo2_) {
                return true;
            }
        }

        return false;
    }
};


// ============================================================
// OPERATORE DI STAMPA PER GLI ARCHI
// ============================================================

template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& e) {
    os << "(" << e.from() << "," << e.to() << ")";
    return os;
}


// ============================================================
// CLASSE unidirected_graph
//
// Rappresenta un grafo non diretto.
// ============================================================

template<typename T>
class unidirected_graph {
private:
    // Per ogni nodo salvo l'insieme dei suoi vicini
    std::map<T, std::set<T>> vicini;

    // Insieme degli archi del grafo
    std::set<unidirected_edge<T>> archi;

    // Lista degli archi nell'ordine in cui sono stati aggiunti
    std::list<unidirected_edge<T>> ordine_archi;

    // Associa a ogni arco un numero
    std::map<unidirected_edge<T>, int> numeri_archi;


    // Aggiorna la numerazione degli archi.
    //
    // Uso "archi", che è uno std::set.
    // Quindi gli archi vengono scanditi in ordine crescente,
    // cioè secondo operator<.
    //
    // Questo è utile per De Pina, perché i vettori booleani
    // devono avere una posizione precisa per ogni arco.
    void update_edge_numbers() {
        numeri_archi.clear();

        int numero = 0;

        for (const auto& arco : archi) {
            numeri_archi[arco] = numero;
            numero = numero + 1;
        }
    }

public:
    // Costruttore di default
    unidirected_graph()
    {}

    // Costruttore di copia
    unidirected_graph(const unidirected_graph<T>& other)
        : vicini(other.vicini),
          archi(other.archi),
          ordine_archi(other.ordine_archi),
          numeri_archi(other.numeri_archi)
    {}


    // ========================================================
    // neighbours
    //
    // Restituisce i vicini di un nodo.
    // ========================================================

    std::set<T> neighbours(const T& nodo) const {
        auto pos = vicini.find(nodo);

        if (pos == vicini.end()) {
            return {};
        }

        return pos->second;
    }


    // ========================================================
    // add_edge
    //
    // Aggiunge un arco al grafo.
    // ========================================================

    void add_edge(const unidirected_edge<T>& arco) {
        auto ret = archi.insert(arco);

        // Se l'arco era già presente, non faccio nulla
        if (!ret.second) {
            return;
        }

        T nodo1 = arco.from();
        T nodo2 = arco.to();

        // Aggiorno la lista dei vicini
        vicini[nodo1].insert(nodo2);
        vicini[nodo2].insert(nodo1);

        // Salvo l'arco anche nella lista degli archi inseriti
        ordine_archi.push_back(arco);

        // Aggiorno la numerazione degli archi
        update_edge_numbers();
    }


    // ========================================================
    // all_edges
    //
    // Restituisce tutti gli archi del grafo.
    // ========================================================

    std::set<unidirected_edge<T>> all_edges() const {
        return archi;
    }


    // ========================================================
    // all_nodes
    //
    // Restituisce tutti i nodi del grafo.
    // ========================================================

    std::set<T> all_nodes() const {
        std::set<T> nodi;

        for (const auto& elemento : vicini) {
            nodi.insert(elemento.first);
        }

        return nodi;
    }


    // ========================================================
    // edge_number
    //
    // Restituisce il numero associato a un arco.
    //
    // Se l'arco non viene trovato, restituisce -1.
    // ========================================================

    int edge_number(const unidirected_edge<T>& arco) const {
        auto pos = numeri_archi.find(arco);

        if (pos == numeri_archi.end()) {
            return -1;
        }

        return pos->second;
    }


    // ========================================================
    // edge_at
    //
    // Restituisce l'arco associato a un certo numero.
    // ========================================================

    unidirected_edge<T> edge_at(int numero) const {
        for (const auto& elemento : numeri_archi) {
            if (elemento.second == numero) {
                return elemento.first;
            }
        }

        std::cout << "Errore: numero arco non valido." << std::endl;

        return *(archi.begin());
    }


    // ========================================================
    // operator-
    //
    // Differenza tra grafi.
    //
    // G - T restituisce un grafo contenente gli archi che stanno
    // in G ma non stanno in T.
    //
    // Serve per costruire il coalbero.
    // ========================================================

    unidirected_graph<T> operator-(const unidirected_graph<T>& other) const {
        unidirected_graph<T> risultato;

        for (const auto& arco : archi) {
            if (other.archi.find(arco) == other.archi.end()) {
                risultato.add_edge(arco);
            }
        }

        return risultato;
    }
};