// creo 10 vettori per ogni algoritmo di ordinamneto calcolo il tempo e faccio la media

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

#include "randfiller.h"
#include "timecounter.h"
#include "sorts.hpp"

struct tempi_sort {
    double bubble = 0;
    double insertion = 0;
    double selection = 0;
    double merge = 0;
    double quick = 0;
    double nuovo_quick = 0;
    double std_sort = 0;
};

int main() {
    randfiller rf;
    timecounter tc;

    ofstream out("sort_times2.dat");
    if (!out) {
        cerr << "Errore nell'apertura del file.\n";
        return 1;
    }

    int num_vec = 10;
    int soglia = 0;
    string algoritmo_soglia = "";

    vector<int> dimensioni;
    vector<tempi_sort> tempi;

    for (int i = 10; i <= 100; i += 10 ) {
        tempi_sort t;

        //creo un vettore di vettori per avere misurazionhi accurate
        vector<vector<int>> vettori(num_vec, vector<int>(i));
        
        for (int j = 0; j < num_vec; j++){
            rf.fill(vettori[j], -1000, 1000);
        }

        //Bubble sort
        vector<vector<int>> vec_bubble = vettori;
        tc.tic();
        for (int j = 0; j < num_vec; j++){
            bubble_sort(vec_bubble[j]);
        }
        t.bubble = tc.toc()/ num_vec;

        //Insertion sort
        vector<vector<int>> vec_insertion = vettori;
        tc.tic();
        for (int j = 0; j < num_vec; j++){
            insertion_sort(vec_insertion[j]);
        }
        t.insertion = tc.toc()/ num_vec;

        //Selection sort
        vector<vector<int>> vec_selection = vettori;
        tc.tic();
        for (int j = 0; j < num_vec; j++){
            selection_sort(vec_selection[j]);
        }
        t.selection = tc.toc()/ num_vec;

        //Merge sort
        vector<vector<int>> vec_merge = vettori;
        tc.tic();
        for (int j = 0; j < num_vec; j++){
            merge_sort(vec_merge[j], 0, vec_merge[j].size() - 1);
        }
        t.merge = tc.toc()/ num_vec;

        //Quick sort
        vector<vector<int>> vec_quick = vettori;
        tc.tic();
        for (int j = 0; j < num_vec; j++){
            quick_sort(vec_quick[j], 0, vec_quick[j].size() - 1);
        }
        t.quick = tc.toc()/ num_vec;

        //std::sort
        vector<vector<int>> vec_std = vettori;
        tc.tic();
        for (int j = 0; j < num_vec; j++) {
            sort(vec_std[j].begin(), vec_std[j].end());
        }
        t.std_sort = tc.toc()/ num_vec;

        //Ricerca dell'n, cioè della soglia
        double quadratico_migliore = t.bubble;
        string alg_quad = "Bubble sort";

        if (t.insertion < quadratico_migliore) {
            quadratico_migliore = t.insertion;
            alg_quad = "Insertion sort";
        }
        if (t.selection < quadratico_migliore){
            quadratico_migliore = t.selection;
            alg_quad = "Selection sort";
        }
        double log_migliore = t.merge;

        if (t.quick < log_migliore){
            log_migliore = t.quick;
        }
        if (quadratico_migliore < log_migliore){
            soglia = i;
            algoritmo_soglia = alg_quad;
        }

        //Salvo dimensioni e tempi misurati
        dimensioni.push_back(i);
        tempi.push_back(t);
    }
    
    if (soglia > 0){
        cout << "Soglia trovata: n = " << soglia << "\n";
        cout << "Algoritmo quadratico scelto: " << algoritmo_soglia << "\n\n";
    }
    else {
        cout << "Nessuna soglia trovata.\n\n";
    }

    //utilizzo la funzione nuovo quick dopo aver trovato la soglia 
    for (size_t k = 0; k < dimensioni.size(); k++) {
        int i = dimensioni[k];

        //nuovo quick
        vector<vector<int>> vettori(num_vec, vector<int>(i));
        for (int j = 0; j < num_vec; j++){
            rf.fill(vettori[j], -1000, 1000);
        }
        //nuovo quick
        vector<vector<int>> vec_nuovoquick = vettori;

        tc.tic();
        for (int j = 0; j < num_vec; j++){
            nuovo_quick(vec_nuovoquick[j], 0, vec_nuovoquick[j].size() - 1, soglia);
        }
        tempi[k].nuovo_quick = tc.toc() / num_vec;
    }
    
    for (size_t k = 0; k < dimensioni.size(); k++){
        cout << "Dimensione: " << dimensioni[k] << "\n";
        cout << "t_bubblesort: " << tempi[k].bubble << "\n";
        cout << "t_insertionsort: " << tempi[k].insertion << "\n"; 
        cout << "t_selectionsort: " << tempi[k].selection << "\n";
        cout << "t_mergesort: " << tempi[k].merge << "\n";
        cout << "t_quicksort: " << tempi[k].quick << "\n";
        cout << "t_nuovoquick: " << tempi[k].nuovo_quick << "\n";
        cout << "t_stdsort: " << tempi[k].std_sort << "\n\n";
        
        out << dimensioni[k] << " " << tempi[k].bubble << " " << tempi[k].insertion << " " << tempi[k].selection << " "<< tempi[k].merge << " "<< tempi[k].quick << " " << tempi[k].nuovo_quick << " " << tempi[k].std_sort << "\n"; 
    }

    out.close();
    cout << "Misure salvate anche nel file 'sort_times2.dat' nel seguente ordine:\n";
    cout << "dimensione, bubble, insertion, selection, merge, quick, nuovoquick, stdsort\n\n";

    
    return 0;
}

