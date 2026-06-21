#pragma once

#include <vector>
#include <map>
#include <iostream>

#include "graph.hpp"
#include "graph_visit.hpp"


// ============================================================
// find_path_visit
//
// Funzione ricorsiva che cerca un percorso tra corrente e
// destinazione dentro l'albero DFS.
//
// Se trova il percorso, restituisce true.
// Se non lo trova, restituisce false.
// ============================================================

template<typename T>
bool find_path_visit(
    const unidirected_graph<T>& albero,
    const T& corrente,
    const T& destinazione,
    std::map<T, bool>& visitati,
    std::vector<T>& percorso
) {
    visitati[corrente] = true;
    percorso.push_back(corrente);

    if (corrente == destinazione) {
        return true;
    }

    for (const auto& vicino : albero.neighbours(corrente)) {
        if (!visitati[vicino]) {
            bool trovato = find_path_visit(
                albero,
                vicino,
                destinazione,
                visitati,
                percorso
            );

            if (trovato) {
                return true;
            }
        }
    }

    percorso.pop_back();

    return false;
}


// ============================================================
// find_path
//
// Funzione wrapper.
// Prepara visitati e percorso, poi chiama find_path_visit.
// ============================================================

template<typename T>
std::vector<T> find_path(
    const unidirected_graph<T>& albero,
    const T& partenza,
    const T& arrivo
) {
    std::map<T, bool> visitati;
    std::vector<T> percorso;

    find_path_visit(
        albero,
        partenza,
        arrivo,
        visitati,
        percorso
    );

    return percorso;
}


// ============================================================
// fundamental_cycles_dfs
//
// Calcola i cicli fondamentali con il metodo DFS.
// ============================================================

template<typename T>
std::vector<std::vector<T>> fundamental_cycles_dfs(
    const unidirected_graph<T>& G,
    const T& sorgente
) {
    std::vector<std::vector<T>> cicli;

    // Calcolo l'albero DFS T = dfs(G)
    unidirected_graph<T> albero = dfs_tree(G, sorgente);

    // Calcolo il coalbero C = G - T
    unidirected_graph<T> coalbero = G - albero;

    // Ogni arco del coalbero genera un ciclo fondamentale
    for (const auto& arco : coalbero.all_edges()) {
        T u = arco.from();
        T v = arco.to();

        // Cerco il percorso tra u e v nell'albero DFS
        std::vector<T> ciclo = find_path(albero, u, v);

        // Richiudo il ciclo usando l'arco del coalbero
        ciclo.push_back(u);

        cicli.push_back(ciclo);
    }

    return cicli;
}


// ============================================================
// print_cicli
//
// Stampa dei cicli.
// ============================================================

template<typename T>
void print_cicli(const std::vector<std::vector<T>>& cicli) {
    for (std::size_t i = 0; i < cicli.size(); i++) {
        std::cout << "Ciclo " << i + 1 << ": ";

        for (const auto& nodo : cicli[i]) {
            std::cout << nodo << " ";
        }

        std::cout << std::endl;
    }
}