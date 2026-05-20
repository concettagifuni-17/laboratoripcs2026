#pragma once
#include <map>
#include <queue>
#include <vector>
#include <utility>
#include <limits>
#include <functional>

#include "graph.hpp"

//siccome il grafo implementato la volta scorsa non ha pesi sugli archi
//l'algoritmo Dijkstra è senza pesi espliciti (considero ogni arco con peso 1)
//ns = nodo sorgente
//np = nodo precedente
template<typename T>
unidirected_graph<T> dijkstra(const unidirected_graph<T>& G, const T& ns) {
    unidirected_graph<T> albero;
    int infinito = std::numeric_limits<int>::max();
    std::map<T, int> distanza;
    std::map<T, T> np;
    //inizializziamo tutte le distanze all'infinito
    for (const T& nodo : G.all_nodes()){
        distanza[nodo] = infinito;
    }
    //la distanza del ns da se stesso è zero
    distanza[ns] = 0;

    //creiamo la coda di priorità che serve a dijkstra per scegliere 
    //sempre il nodo con distanza più piccola
    //mettiamo davanti sempre le coppie con distanza più piccola 
    //coppie (distanza, nodo)
    std::priority_queue<
    std::pair<int, T>,                   //tipo dell'elemento: int per distanze e T per nodo 
    std::vector<std::pair<int, T>>,      //contenitore interno (vettore usato per salvare le coppie)
    std::greater<std::pair<int, T>>      //criterio di ordine: davanti la coppia più piccola (con distanze minori)
    > prio_queue;
    
    prio_queue.push({0, ns}); //inserisce il primo nodo da visitare, coiè il nodo sorgente

    while (!prio_queue.empty()) {
        T n = prio_queue.top().second;     //nodo con distanza minima
        int d = prio_queue.top().first;    //distanza del nodo
        prio_queue.pop();

        //se questa diestanza non è più aggiornata, salto il nodo
        if (d > distanza[n]) {
            continue;
        }
        //guardo tutti i vicini del nodo n
        for (const T& w : G.neighbours(n)) {
            //distanza minima del nodo n + 1 perchè il 'peso' di ogni arco è 1 = distanza per arrivare al nodo successivo passando per n
            //confronto con la distanza che già avevo per arrivare a w per vedere se è migliore 
            if (distanza[n] +1 < distanza[w]) { 
                distanza[w] = distanza[n] + 1;  //se è così aggiorno
                np[w] = n;  //aggiorno anche il nodo precedente 
                //aggiorno la coda
                prio_queue.push({distanza[w], w});
            }
        }
    }
    //costruisco l'albero finale usando i predecessori trovati
    for (const auto& elemento : np) {
        T nodo = elemento.first;
        T precedente = elemento.second;
        albero.add_edge(unidirected_edge<T>(precedente, nodo));
    }

    return albero;
}