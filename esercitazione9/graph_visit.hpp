#pragma once
#include <queue>
#include <stack>
#include <map>
#include "graph.hpp"

//Contenitore FIFO, serve per BFS
template<typename T>
class fifo {
    std::queue<T> q;
public:
    fifo() {}
    void put(const T& valore) {
        q.push(valore);
    }
    T get() {
        T num = q.front();
        q.pop();
        return num;
    }
    bool empty() const {
        return q.empty();
    }
};
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

//funzione unica per BFS e DFS iterativa
//ns = nodo sorgente 
template<typename T, typename Container>
unidirected_graph<T> graph_visit(const unidirected_graph<T>& G, const T& ns, Container& contenitore) {
    unidirected_graph<T> albero;
    std::map<T, bool> visitati;

    contenitore.put(ns);  //metto il nodo sorgente nel contenitore
    visitati[ns] = true;  //lo segno come visitato

    while (!contenitore.empty()) {
        T nodo = contenitore.get(); //estraggo un nodo dal contenitore
        for (const auto& vicino : G.neighbours(nodo)) {    //guardo tutti i suoi vicini
            if (!visitati[vicino]) {                 //se il vicino non è ancora stato visitato
                visitati[vicino] = true;             //lo segno come visitato
                //aggiungo all'albero l'arco con cui ho raggiunto il nodo vicino           
                albero.add_edge(unidirected_edge<T>(nodo, vicino));
                contenitore.put(vicino);             //inserisco il vicino nel contenitore
            }
        }
    }
    return albero;
}