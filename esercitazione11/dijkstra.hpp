// Algoritmo di Dijkstra per trovare il cammino minimo tra due nodi.
// Versione modificata del dijkstra.hpp usato nell'esercitazione 9.

#pragma once

#include <map>
#include <queue>
#include <vector>
#include <utility>
#include <limits>
#include <functional>
#include <algorithm>

#include "graph.hpp"


// Questa funzione non restituisce un albero,
// ma restituisce il cammino minimo da ns a nd.
template<typename T>
std::vector<T> dijkstra(
    const unidirected_graph<T>& G,
    const T& ns,
    const T& nd
) {
    int infinito = std::numeric_limits<int>::max();

    std::map<T, int> distanza;
    std::map<T, T> np;

    for (const T& nodo : G.all_nodes()) {
        distanza[nodo] = infinito;
    }

    distanza[ns] = 0;

    std::priority_queue<
        std::pair<int, T>,
        std::vector<std::pair<int, T>>,
        std::greater<std::pair<int, T>>
    > prio_queue;

    prio_queue.push({0, ns});

    while (!prio_queue.empty()) {
        T n = prio_queue.top().second;
        int d = prio_queue.top().first;

        prio_queue.pop();

        if (d > distanza[n]) {
            continue;
        }

        if (n == nd) {
            break;
        }

        for (const T& w : G.neighbours(n)) {
            if (distanza[n] + 1 < distanza[w]) {
                distanza[w] = distanza[n] + 1;
                np[w] = n;

                prio_queue.push({distanza[w], w});
            }
        }
    }

    std::vector<T> percorso;

    if (distanza[nd] == infinito) {
        return percorso;
    }

    T nodo = nd;

    while (!(nodo == ns)) {
        percorso.push_back(nodo);
        nodo = np[nodo];
    }

    percorso.push_back(ns);

    std::reverse(percorso.begin(), percorso.end());

    return percorso;
}