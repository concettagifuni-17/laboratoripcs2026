#include <iostream>
#include <vector>

#include "circuit.hpp"
using namespace std;

int main(int argc, const char* argv[]) {
    vector<Component> components;

    // Se passo il nome del file da terminale, legge da file.
    // Altrimenti legge da input standard.
    if (argc >= 2) {
        components = read_netlist_file(argv[1]);
    } else {
        cerr << "Netlist mancante." << "\n";
    }

    // Se la lettura fallisce, read_netlist restituisce un vettore vuoto.
    if (components.empty()) {
        return 1;
    }

    Circuit circuit = build_circuit(components);

    if (!circuit.valid) {
        return 1;
    }

    vector<ResistorResult> results = solve_circuit_checked(circuit);

    if (results.empty()) {
        return 1;
    }

    print_resistor_results(cout, results);

    return 0;
}