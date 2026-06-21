#include <iostream>

#include "graph.hpp"
#include "graph_visit.hpp"
#include "cicli_fondamentali.hpp"
#include "boolean_vector.hpp"
#include "dijkstra.hpp"
#include "depina.hpp"


int main() {
    // Creo un grafo non diretto
    unidirected_graph<int> G;

    // Aggiungo gli archi del grafo
    //
    // Grafo usato:
    //
    // 0 ----- 1
    // |       |
    // |       |
    // 4 ----- 3 ----- 2
    //         \     /
    //          \   /
    //            2
    //
    // In pratica uso un ciclo grande e una diagonale.
    //
    // Archi:
    // (0,1), (0,4), (1,2), (1,3), (2,3), (3,4)

    G.add_edge(unidirected_edge<int>(0, 1));
    G.add_edge(unidirected_edge<int>(0, 4));
    G.add_edge(unidirected_edge<int>(1, 2));
    G.add_edge(unidirected_edge<int>(1, 3));
    G.add_edge(unidirected_edge<int>(2, 3));
    G.add_edge(unidirected_edge<int>(3, 4));


    std::cout << "==============================" << "\n";
    std::cout << "GRAFO" << "\n";
    std::cout << "==============================" << "\n";

    print_numbered_edges(G);


    std::cout << "\n";
    std::cout << "Numero nodi: " << G.all_nodes().size() << "\n";
    std::cout << "Numero archi: " << G.all_edges().size() << "\n";

    int k = static_cast<int>(G.all_edges().size())
          - static_cast<int>(G.all_nodes().size())
          + 1;

    std::cout << "Numero cicli attesi k = m - n + 1 = "
              << k
              << "\n";


    std::cout << "\n";
    std::cout << "==============================" << "\n";
    std::cout << "CICLI FONDAMENTALI CON DFS" << "\n";
    std::cout << "==============================" << "\n";

    std::vector<std::vector<int>> cicli =
        fundamental_cycles_dfs(G, 0);

    print_cicli(cicli);


    std::cout << "\n";
    std::cout << "==============================" << "\n";
    std::cout << "BASE DI CICLI MINIMI CON DE PINA" << "\n";
    std::cout << "==============================" << "\n";

    std::vector<BoolVector<int>> base =
        depina_minimum_cycle_basis(G, 0);

    print_cycle_basis(base);


    return 0;
}