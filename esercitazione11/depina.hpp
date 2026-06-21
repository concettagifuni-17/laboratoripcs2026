// algoritmo di De Pina
// si cerca una base di cicli minimi

#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <limits>

#include "graph.hpp"
#include "graph_visit.hpp"
#include "boolean_vector.hpp"
#include "dijkstra.hpp"


// ============================================================
// AuxNode: nodi ausiliari
//
// Nel grafo ausiliario G' ogni nodo v viene duplicato:
// v+ = (v, 1)
// v- = (v, 0)
// ============================================================

template<typename T>
using AuxNode = std::pair<T, int>;


// nodo v+
template<typename T>
AuxNode<T> nodo_positivo(const T& nodo) {
    return {nodo, 1};
}


// nodo v-
template<typename T>
AuxNode<T> nodo_negativo(const T& nodo) {
    return {nodo, 0};
}


// ============================================================
// Inizializzazione dei vettori S_i
//
// Creo un vettore booleano per ogni arco del coalbero.
// Ogni vettore ha lunghezza m = numero di archi di G.
// In ogni vettore accendo con 1 la posizione dell'arco
// del coalbero corrispondente.
// ============================================================

template<typename T>
std::vector<BoolVector<int>> initialize_S_vectors(
    const unidirected_graph<T>& G,
    const unidirected_graph<T>& coalbero
) {
    int numero_archi = static_cast<int>(G.all_edges().size());

    std::vector<BoolVector<int>> vettori_S;

    for (const auto& arco : coalbero.all_edges()) {
        BoolVector<int> vettore(numero_archi, 0);

        int posizione = G.edge_number(arco);

        if (posizione >= 0) {
            vettore[posizione] = 1;
        }

        vettori_S.push_back(vettore);
    }

    return vettori_S;
}


// ============================================================
// Costruzione del grafo ausiliario G'
//
// Per ogni arco (u,v) di G:
//
// se l'arco è attivo in S_i:
//     aggiungo (u+, v-) e (u-, v+)
//
// se l'arco non è attivo in S_i:
//     aggiungo (u+, v+) e (u-, v-)
// ============================================================

template<typename T>
unidirected_graph<AuxNode<T>> build_auxiliary_graph(
    const unidirected_graph<T>& G,
    const BoolVector<int>& S_i
) {
    unidirected_graph<AuxNode<T>> grafo_ausiliario;

    for (const auto& arco : G.all_edges()) {
        T u = arco.from();
        T v = arco.to();

        int posizione = G.edge_number(arco);

        bool arco_attivo = false;

        if (posizione >= 0 && posizione < static_cast<int>(S_i.size())) {
            if (S_i[posizione] == 1) {
                arco_attivo = true;
            }
        }

        AuxNode<T> u_piu = nodo_positivo(u);
        AuxNode<T> u_meno = nodo_negativo(u);

        AuxNode<T> v_piu = nodo_positivo(v);
        AuxNode<T> v_meno = nodo_negativo(v);

        if (arco_attivo) {
            grafo_ausiliario.add_edge(
                unidirected_edge<AuxNode<T>>(u_piu, v_meno)
            );

            grafo_ausiliario.add_edge(
                unidirected_edge<AuxNode<T>>(u_meno, v_piu)
            );
        } else {
            grafo_ausiliario.add_edge(
                unidirected_edge<AuxNode<T>>(u_piu, v_piu)
            );

            grafo_ausiliario.add_edge(
                unidirected_edge<AuxNode<T>>(u_meno, v_meno)
            );
        }
    }

    return grafo_ausiliario;
}


// ============================================================
// Conversione di un cammino in G' in vettore di incidenza
//
// Un cammino in G' contiene nodi del tipo (nodo_originale, segno).
// A noi interessa solo il nodo originale.
// Ogni coppia consecutiva individua un arco del grafo originale.
// ============================================================

template<typename T>
BoolVector<int> auxiliary_path_to_incidence_vector(
    const unidirected_graph<T>& G,
    const std::vector<AuxNode<T>>& percorso_ausiliario
) {
    int numero_archi = static_cast<int>(G.all_edges().size());

    BoolVector<int> incidenza(numero_archi, 0);

    for (std::size_t i = 0; i + 1 < percorso_ausiliario.size(); i++) {
        T nodo1 = percorso_ausiliario[i].first;
        T nodo2 = percorso_ausiliario[i + 1].first;

        if (nodo1 == nodo2) {
            continue;
        }

        unidirected_edge<T> arco_originale(nodo1, nodo2);

        int posizione = G.edge_number(arco_originale);

        if (posizione >= 0) {
            incidenza[posizione] = (incidenza[posizione] + 1) % 2;
        }
    }

    return incidenza;
}


// ============================================================
// Conversione di un ciclo scritto come lista di nodi
// in vettore booleano di incidenza.
// ============================================================

template<typename T>
BoolVector<int> cycle_to_incidence_vector(
    const unidirected_graph<T>& G,
    const std::vector<T>& ciclo
) {
    int numero_archi = static_cast<int>(G.all_edges().size());

    BoolVector<int> incidenza(numero_archi, 0);

    for (std::size_t i = 0; i + 1 < ciclo.size(); i++) {
        unidirected_edge<T> arco(ciclo[i], ciclo[i + 1]);

        int posizione = G.edge_number(arco);

        if (posizione >= 0) {
            incidenza[posizione] = (incidenza[posizione] + 1) % 2;
        }
    }

    return incidenza;
}


// ============================================================
// Ricerca del ciclo minimo tale che <C_i, S_i> = 1
// ============================================================

template<typename T>
BoolVector<int> find_minimal_cycle(
    const unidirected_graph<T>& G,
    const BoolVector<int>& S_i
) {
    int numero_archi = static_cast<int>(G.all_edges().size());

    BoolVector<int> ciclo_migliore(numero_archi, 0);

    int peso_migliore = std::numeric_limits<int>::max();

    unidirected_graph<AuxNode<T>> grafo_ausiliario =
        build_auxiliary_graph(G, S_i);

    for (const auto& nodo : G.all_nodes()) {
        AuxNode<T> sorgente = nodo_negativo(nodo);
        AuxNode<T> destinazione = nodo_positivo(nodo);

        std::vector<AuxNode<T>> percorso_ausiliario =
            dijkstra(
                grafo_ausiliario,
                sorgente,
                destinazione
            );

        if (percorso_ausiliario.empty()) {
            continue;
        }

        BoolVector<int> candidato =
            auxiliary_path_to_incidence_vector(
                G,
                percorso_ausiliario
            );

        int prodotto = scalar_product_mod2<int>(
            candidato,
            S_i
        );

        if (prodotto == 1) {
            int peso = count_ones<int>(candidato);

            if (peso < peso_migliore) {
                peso_migliore = peso;
                ciclo_migliore = candidato;
            }
        }
    }

    if (peso_migliore == std::numeric_limits<int>::max()) {
        std::cout << "Attenzione: nessun ciclo minimo trovato." << "\n";
    }

    return ciclo_migliore;
}


// ============================================================
// Algoritmo di De Pina vero e proprio
// ============================================================

template<typename T>
std::vector<BoolVector<int>> depina_minimum_cycle_basis(
    const unidirected_graph<T>& G,
    const T& sorgente
) {
    std::vector<BoolVector<int>> base;

    int numero_archi = static_cast<int>(G.all_edges().size());
    int numero_nodi = static_cast<int>(G.all_nodes().size());

    int k = numero_archi - numero_nodi + 1;

    unidirected_graph<T> albero = dfs_tree(G, sorgente);

    unidirected_graph<T> coalbero = G - albero;

    std::vector<BoolVector<int>> vettori_S =
        initialize_S_vectors(G, coalbero);

    if (static_cast<int>(vettori_S.size()) != k) {
        std::cout << "Errore: il coalbero non contiene k archi." << "\n";
        return base;
    }

    for (int i = 0; i < k; i++) {
        BoolVector<int> ciclo_i =
            find_minimal_cycle(G, vettori_S[i]);

        base.push_back(ciclo_i);

        for (int j = i + 1; j < k; j++) {
            int prodotto = scalar_product_mod2<int>(
                ciclo_i,
                vettori_S[j]
            );

            if (prodotto == 1) {
                vettori_S[j] = xor_vector<int>(
                    vettori_S[j],
                    vettori_S[i]
                );
            }
        }
    }

    return base;
}


// ============================================================
// Stampa della base di cicli minimi
// ============================================================

void print_cycle_basis(
    const std::vector<BoolVector<int>>& base
) {
    for (std::size_t i = 0; i < base.size(); i++) {
        std::cout << "Ciclo minimo " << i + 1 << ": ";

        print_bool_vector<int>(base[i]);

        std::cout << " peso = " << count_ones<int>(base[i]);

        std::cout << "\n";
    }
}


// ============================================================
// Stampa degli archi numerati
// ============================================================

template<typename T>
void print_numbered_edges(
    const unidirected_graph<T>& G
) {
    std::cout << "Archi numerati:" << "\n";

    for (const auto& arco : G.all_edges()) {
        std::cout << G.edge_number(arco)
                  << " -> "
                  << arco
                  << "\n";
    }
}