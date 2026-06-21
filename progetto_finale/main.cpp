#include <iostream>
#include <vector>

#include "circuit.hpp"
using namespace std;

int main(int argc, const char* argv[]) {
    vector<Componenti> componenti;

    if (argc >= 2) {
        componenti = read_netlist_file(argv[1]);
    } else {
        cerr << "Errore: Netlist mancante.\n";
        cerr << "Uso: ./main nome_file_netlist.txt\n";
        return 1;
    }

    // Se la lettura fallisce, read_netlist restituisce un vettore vuoto.
    if (componenti.empty()) {
        return 1;
    }

    Circuiti circuito = build_circuit(componenti);

    if (!circuito.valid) {
        return 1;
    }

    vector<RisultatiR> risultati = solve_circuit_checked(circuito);

    if (risultati.empty()) {
        return 1;
    }

    print_resistor_results(cout, risultati);

    return 0;
}