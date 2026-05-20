#pragma once
#include <map>
#include "graph.hpp"

//funzione ricorsiva usata dalla DFS
template<typename T>
void recursive_dfs_visit(const unidirected_graph<T>& G, const T& nodo, std::map<T, bool>& visitati, unidirected_graph<T>& albero) {
    visitati[nodo] = true;                                        //segno il nodo corrente come visitato
    for (const auto& vicino : G.neighbours(nodo)) {               //guardo tutti i suoi vicini
        if (!visitati[vicino]) {                                  //se il vicino non è ancora stato visitato
            //aggiungo all'albero l'arco con cui ho raggiunto il nodo vicino
            albero.add_edge(unidirected_edge<T>(nodo, vicino));
            //continuo la visita in profondità dal vicino
            recursive_dfs_visit(G, vicino, visitati, albero);
        }
    }
}
//sorgente = nodo da cui parte la DFS ricorsiva
template<typename T>
unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& G, const T& sorgente) {
    unidirected_graph<T> albero;                    //grafo in cui salvo l'albero della DFS ricorsiva
    std::map<T, bool> visitati;                     //mappa per ricordare quali nodi sono già stati visitati 
    //avvio la visita ricorsiva partndo dal nodo sorgente
    recursive_dfs_visit(G, sorgente, visitati, albero);
    return albero;
}