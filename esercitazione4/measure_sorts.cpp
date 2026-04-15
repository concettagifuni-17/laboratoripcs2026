#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

#include "randfiller.h"
#include "timecounter.h"
#include "sorts.hpp"

int main() {
    randfiller rf;
    timecounter tc;

    ofstream out("sort_times.dat");
    if (!out) {
        cerr << "Errore nell'apertura del file.\n";
        return 1;
    }

    for (int i = 4; i <= 8192; i= i*2 ) {
        vector<int> vec(i);
        
        rf.fill(vec, -1000, 1000);
        tc.tic();
        bubble_sort(vec);
        double t_bubble = tc.toc();
        
        rf.fill(vec, -1000, 1000);
        tc.tic();
        insertion_sort(vec);
        double t_insertion = tc.toc();

        rf.fill(vec, -1000, 1000);
        tc.tic();
        selection_sort(vec);
        double t_selection = tc.toc();

        rf.fill(vec, -1000, 1000);
        tc.tic();
        sort(vec.begin(), vec.end());
        double t_sort = tc.toc();

        cout << "Dimensione: " << i << "\n";
        cout << "t_bubble: " << t_bubble << "\n";
        cout << "t_insertion: " << t_insertion << "\n"; 
        cout << "t_selection: " << t_selection << "\n"; 
        cout << "t_sort: " << t_sort << "\n\n";
        
        out << i << " " << t_bubble << " " << t_insertion << " " << t_selection << " " << t_sort << "\n"; 
    }

    out.close();
    cout << "Misure salvate anche nel file 'sort_times.dat' nel seguente ordine: dimensione, bubble, insertion, selection, standard sort" << "\n";

    return 0;
}