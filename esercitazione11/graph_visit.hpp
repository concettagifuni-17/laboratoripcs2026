#pragma once
#include <queue>
#include <stack>
#include <map>
#include "graph.hpp"

//Contenitore LIFO, serve per DFS iterativa
template<typename T>
class lifo {
    std::stack<T> s;
public:
    lifo() {}
    void put(const T& valore) {
        s.push(valore);
    }
    T get() {
        T num = s.top();
        s.pop();
        return num;
    }
    bool empty() const {
        return s.empty();
    }
};

//iterativa: costruisce l'albero DFS T = dfs(G)
template<typename T>
unidirected_graph<T> dfs_tree(const unidirected_graph<T>& G, const T& sorgente) {
    unidirected_graph<T> albero;
    std::map<T, bool> visitati;
    lifo<T> pila;

    pila.put(sorgente);  //metto il nodo sorgente nel contenitore
    visitati[sorgente] = true;  //lo segno come visitato

    while (!pila.empty()) {
        T nodo = pila.get(); //estraggo un nodo dal contenitore
        for (const auto& vicino : G.neighbours(nodo)) {    //guardo tutti i suoi vicini
            if (!visitati[vicino]) {                 //se il vicino non è ancora stato visitato
                visitati[vicino] = true;             //lo segno come visitato
                //aggiungo all'albero l'arco con cui ho raggiunto il nodo vicino           
                albero.add_edge(unidirected_edge<T>(nodo, vicino));
                pila.put(vicino);             //inserisco il vicino nel contenitore
            }
        }
    }
    return albero;
}