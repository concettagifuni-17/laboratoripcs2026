#pragma once
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <Eigen/Dense>

#include "graph.hpp"
#include "cicli_fondamentali.hpp"
#include "depina.hpp"
#include "grad_coniugato.hpp"

using EdgeKey = std::pair<int,int>;
using NodeCycle = std::vector<int>;

enum class ComponentType { Resistor, VoltageSource };
enum class CycleMethod { Dfs, DePina };

struct Component {
    std::string name; ComponentType type = ComponentType::Resistor;
    double value = 0.0; int node1 = 0; int node2 = 0;
};
struct ResistorResult { std::string name; double voltage = 0.0; double current = 0.0; };
struct Circuit {
    bool valid = true;
    std::vector<Component> components, resistors, generators;
    std::map<EdgeKey,int> resistor_on_edge, generator_on_edge;
    undirected_graph<int> graph;
};
struct MeshSolution { bool valid = true; std::vector<ResistorResult> results; };

inline bool errore(const std::string& s) { std::cerr << "Errore: " << s << "\n"; return false; }
inline EdgeKey edge_key(int a, int b) { return (a < b) ? EdgeKey{a,b} : EdgeKey{b,a}; }
inline int verso_arco(int from, int to) { return (from < to) ? 1 : -1; }
inline bool vicino(double a, double b) {
    double scala = 1.0 + std::max(std::abs(a), std::abs(b));
    return std::abs(a - b) <= 1.0e-7 * scala;
}

inline std::string togli_commento(const std::string& riga) {
    std::size_t p1 = riga.find('#'), p2 = riga.find("//"), p = std::string::npos;
    if (p1 != std::string::npos) p = p1;
    if (p2 != std::string::npos && (p == std::string::npos || p2 < p)) p = p2;
    return (p == std::string::npos) ? riga : riga.substr(0, p);
}
inline bool riga_vuota(const std::string& riga) {
    for (char c : riga) if (!std::isspace(static_cast<unsigned char>(c))) return false;
    return true;
}

inline std::vector<Component> read_netlist(std::istream& in) {
    std::vector<Component> v; std::set<std::string> nomi; std::string riga; int nriga = 0;
    while (std::getline(in, riga)) {
        nriga++; riga = togli_commento(riga); if (riga_vuota(riga)) continue;
        Component c; std::string extra; std::istringstream iss(riga);
        if (!(iss >> c.name >> c.value >> c.node1 >> c.node2) || (iss >> extra)) {
            errore("riga " + std::to_string(nriga) + " non valida"); return {};
        }
        if (c.name.empty() || (c.name[0]!='R' && c.name[0]!='r' && c.name[0]!='V' && c.name[0]!='v')) {
            errore("componente " + c.name + " non valido"); return {};
        }
        if (nomi.find(c.name) != nomi.end()) { errore("nome duplicato: " + c.name); return {}; }
        if (c.node1 == c.node2) { errore("componente " + c.name + " collegato allo stesso nodo"); return {}; }
        c.type = (c.name[0]=='R' || c.name[0]=='r') ? ComponentType::Resistor : ComponentType::VoltageSource;
        if (c.type == ComponentType::Resistor && c.value <= 0.0) { errore("resistenza non positiva: " + c.name); return {}; }
        nomi.insert(c.name); v.push_back(c);
    }
    if (v.empty()) errore("netlist vuota");
    return v;
}

inline std::vector<Component> read_netlist_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) { errore("impossibile aprire " + filename); return {}; }
    return read_netlist(in);
}

inline bool component_less(const Component& a, const Component& b) {
    EdgeKey ea = edge_key(a.node1,a.node2), eb = edge_key(b.node1,b.node2);
    return (ea == eb) ? a.name < b.name : ea < eb;
}

inline bool connected_graph(const undirected_graph<int>& g) {
    std::set<int> nodi = g.all_nodes(); if (nodi.empty()) return false;
    std::set<int> visti; std::vector<int> stack;
    stack.push_back(*nodi.begin()); visti.insert(*nodi.begin());
    while (!stack.empty()) {
        int u = stack.back(); stack.pop_back();
        for (int x : g.neighbours(u)) if (visti.find(x) == visti.end()) { visti.insert(x); stack.push_back(x); }
    }
    return visti.size() == nodi.size();
}

inline Circuit build_circuit(const std::vector<Component>& input) {
    Circuit c; c.components = input; std::sort(c.components.begin(), c.components.end(), component_less);
    std::set<EdgeKey> usati;
    for (const Component& x : c.components) {
        EdgeKey edge = edge_key(x.node1,x.node2);
        if (usati.find(edge) != usati.end()) { c.valid = false; errore("componenti in parallelo"); return c; }
        usati.insert(edge); c.graph.add_edge(undirected_edge<int>(edge.first,edge.second));
        if (x.type == ComponentType::Resistor) { c.resistor_on_edge[edge] = static_cast<int>(c.resistors.size()); c.resistors.push_back(x); }
        else { c.generator_on_edge[edge] = static_cast<int>(c.generators.size()); c.generators.push_back(x); }
    }
    int cicli = static_cast<int>(c.graph.all_edges().size()) - static_cast<int>(c.graph.all_nodes().size()) + 1;
    if (c.resistors.empty()) { c.valid = false; errore("nessun resistore"); }
    if (!connected_graph(c.graph)) { c.valid = false; errore("grafo non connesso"); }
    if (cicli <= 0) { c.valid = false; errore("nessuna maglia indipendente"); }
    return c;
}

inline NodeCycle incidence_to_cycle(const undirected_graph<int>& g, const BoolVector<int>& inc) {
    std::map<int,std::vector<int>> adj; int m = 0;
    for (int i = 0; i < static_cast<int>(inc.size()); i++) if (inc[i] != 0) {
        undirected_edge<int> edge = g.edge_at(i); adj[edge.from()].push_back(edge.to()); adj[edge.to()].push_back(edge.from()); m++;
    }
    if (m == 0) return {};
    for (const auto& x : adj) if (x.second.size() != 2) return {};
    int start = adj.begin()->first, prev = start, cur = start, next = adj[start][0];
    NodeCycle ciclo; ciclo.push_back(start);
    for (int k = 0; k <= m; k++) {
        ciclo.push_back(next); prev = cur; cur = next;
        if (cur == start) return (static_cast<int>(ciclo.size()) == m + 1) ? ciclo : NodeCycle{};
        next = (adj[cur][0] == prev) ? adj[cur][1] : adj[cur][0];
    }
    return {};
}

inline std::vector<NodeCycle> get_cycles(const Circuit& c, CycleMethod metodo) {
    int attesi = static_cast<int>(c.graph.all_edges().size()) - static_cast<int>(c.graph.all_nodes().size()) + 1;
    int sorgente = *c.graph.all_nodes().begin(); std::vector<NodeCycle> cicli;
    if (metodo == CycleMethod::Dfs) cicli = fundamental_cycles_dfs(c.graph, sorgente);
    else {
        for (const BoolVector<int>& inc : depina_minimum_cycle_basis(c.graph, sorgente)) {
            NodeCycle ciclo = incidence_to_cycle(c.graph, inc); if (ciclo.empty()) return {}; cicli.push_back(ciclo);
        }
    }
    if (static_cast<int>(cicli.size()) != attesi) return {};
    for (const NodeCycle& ciclo : cicli) if (ciclo.size() < 2 || ciclo.front() != ciclo.back()) return {};
    return cicli;
}

inline double contributo_generatore(const Component& g, int from, int to, bool& ok) {
    ok = true;
    if (from == g.node2 && to == g.node1) return g.value;
    if (from == g.node1 && to == g.node2) return -g.value;
    ok = false; return 0.0;
}

inline MeshSolution solve_method(const Circuit& c, CycleMethod metodo) {
    MeshSolution s; if (!c.valid) { s.valid = false; return s; }
    std::vector<NodeCycle> cicli = get_cycles(c, metodo);
    if (cicli.empty()) { s.valid = false; errore("cicli non trovati"); return s; }

    int nr = static_cast<int>(c.resistors.size()), nc = static_cast<int>(cicli.size());
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(nr,nc);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(nc);

    for (int j = 0; j < nc; j++) for (std::size_t k = 0; k + 1 < cicli[j].size(); k++) {
        int from = cicli[j][k], to = cicli[j][k+1]; EdgeKey edge = edge_key(from,to);
        auto rp = c.resistor_on_edge.find(edge);
        auto gp = c.generator_on_edge.find(edge);
        if (rp != c.resistor_on_edge.end()) B(rp->second,j) += verso_arco(from,to);
        else if (gp != c.generator_on_edge.end()) {
            bool ok = true; b(j) += contributo_generatore(c.generators[gp->second], from, to, ok);
            if (!ok) { s.valid = false; return s; }
        }
    }

    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(nc,nc);
    for (int i = 0; i < nc; i++) for (int j = 0; j < nc; j++) for (int r = 0; r < nr; r++)
        A(i,j) += B(r,i) * c.resistors[r].value * B(r,j);

    unsigned int it = 0; bool converged = false;
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(nc);
    Eigen::VectorXd im = conjugate_gradient(A, b, x0, 1.0e-12, 10000, it, converged);
    if (!converged) { s.valid = false; errore("gradiente coniugato non convergente"); return s; }

    for (int r = 0; r < nr; r++) {
        double I = 0.0; for (int j = 0; j < nc; j++) I += B(r,j) * im(j);
        s.results.push_back({c.resistors[r].name, c.resistors[r].value * I, I});
    }
    return s;
}

inline std::vector<ResistorResult> solve_circuit_checked(const Circuit& c) {
    MeshSolution dfs = solve_method(c, CycleMethod::Dfs), dep = solve_method(c, CycleMethod::DePina);
    if (!dfs.valid || !dep.valid || dfs.results.size() != dep.results.size()) return {};
    for (std::size_t i = 0; i < dfs.results.size(); i++) {
        if (dfs.results[i].name != dep.results[i].name || !vicino(dfs.results[i].voltage, dep.results[i].voltage) || !vicino(dfs.results[i].current, dep.results[i].current)) {
            errore("DFS e De Pina non coincidono su " + dfs.results[i].name); return {};
        }
    }
    return dep.results;
}

inline std::string format_number(double x) {
    if (std::abs(x) < 1.0e-10) {
        x = 0.0;
    }
    if (std::abs(x - std::round(x)) < 1.0e-9) {
        return std::to_string(static_cast<long long>(std::llround(x))); 
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << x;
    std::string s = out.str(); 
    while (!s.empty() && s.back() == '0') {
        s.pop_back();
    }
    if (!s.empty() && s.back() == '.') {
        s.pop_back();
    } 
    if (s == "-0") {
        return "0";
    }
    return s;
}

inline void print_resistor_results(std::ostream& out, const std::vector<ResistorResult>& risultati) {
    for (const ResistorResult& r : risultati)
        out << r.name << ": V = " << format_number(r.voltage) << " volts, I = " << format_number(r.current) << " amps.\n";
}