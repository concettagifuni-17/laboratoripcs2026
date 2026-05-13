#include <iostream>
#include "graph.hpp"
using namespace std;

int main(void) {
    unidirected_edge<int> e1(5,2);
    unidirected_edge<int> e2(2,5);
    unidirected_edge<int> e3(1,4);

    cout << "Test della classe unidirected_edge:\n";
    cout << "e1 = " << e1 << "\n";
    cout << "e2 = " << e2 << "\n";
    cout << "e3 = " << e3 << "\n\n";

    cout << "Estremi di e1:\n";
    cout << "from(e1) = " << e1.from() << "\n";
    cout << "to(e1) = " << e1.to() << "\n\n";

    cout << "Confronto tra archi:\n";
    cout << "e1 = e2 : ";
    if (e1 == e2) {
        cout << "true\n";
    } 
    else {
        cout << "false\n";
    }

    cout << "e1 < e3 : ";
    if (e1 < e3) {
        cout << "true\n\n";
    } 
    else {
        cout << "false\n\n";
    }

    unidirected_graph<int> g;
    g.add_edge(unidirected_edge<int>(1,2));
    g.add_edge(unidirected_edge<int>(1,3));
    g.add_edge(unidirected_edge<int>(2,4));

    cout << "Test della classe unidirected_graph:\n";
    cout << "Grafo g dopo l'inserimento degli archi (1,2), (1,3), (2,4):\n";

    cout << "Nodi di g:\n";
    for (const auto& nodo : g.all_nodes()) {
        cout << nodo << " ";
    }
    cout << "\n";

    cout << "Archi di g:\n";
    for (const auto& arco : g.all_edges()) {
        cout << arco << "\n";
    }
    cout << "\n";

    cout << "Aggiungo di nuovo l'arco (2,1), che equivale a (1,2):\n";
    g.add_edge(unidirected_edge<int>(2,1));

    cout << "Archi di g dopo l'inserimento dell'arco equivalente:\n";
    for (const auto& arco : g.all_edges()) {
        cout << arco << "\n";
    }
    cout << "\n";

    cout << "Vicini del nodo 1: ";
    for (const auto& vicino : g.neighbours(1)) {
        cout << vicino << " ";
    }
    cout << "\n";

    cout << "Vicini del nodo 2: ";
    for (const auto& vicino : g.neighbours(2)) {
        cout << vicino << " ";
    }
    cout << "\n";

    cout << "Vicini del nodo 3: ";
    for (const auto& vicino : g.neighbours(3)) {
        cout << vicino << " ";
    }
    cout << "\n";

    cout << "Vicini del nodo 4: ";
    for (const auto& vicino : g.neighbours(4)) {
        cout << vicino << " ";
    }
    cout << "\n\n";

    cout << "Numerazione degli archi:\n";
    cout << "edge_number((1,2)) = " << g.edge_number(unidirected_edge<int>(1,2)) << "\n";
    cout << "edge_number((1,3)) = " << g.edge_number(unidirected_edge<int>(1,3)) << "\n";
    cout << "edge_number((2,4)) = " << g.edge_number(unidirected_edge<int>(2,4)) << "\n\n";

    cout << "Archi recuperati tramite edge_at:\n";
    cout << "edge_at(0) = " << g.edge_at(0) << "\n";
    cout << "edge_at(1) = " << g.edge_at(1) << "\n";
    cout << "edge_at(2) = " << g.edge_at(2) << "\n\n";

    cout << "Test della differenza tra grafi:\n";

    unidirected_graph<int> h;
    h.add_edge(unidirected_edge<int>(1,2));
    h.add_edge(unidirected_edge<int>(2,4));

    cout << "Archi di h:\n";
    for (const auto& arco : h.all_edges()) {
        cout << arco << "\n";
    }
    cout << "\n";

    unidirected_graph<int> differenza = g - h;

    cout << "Archi di g - h:\n";
    for (const auto& arco : differenza.all_edges()) {
        cout << arco << "\n";
    }

    return 0;
}