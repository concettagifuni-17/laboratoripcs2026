#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

#include "randfiller.h"
#include "timecounter.h"
#include "sorts.hpp"

int main() {
    randfiller rf;
    timecounter tc;

    for (int i = 4; i <= 8192; i= i*2 ) {
        vector<int> vec(i);
        rf.fill(vec, -1000, 1000);

        vector<int> v_bubble = vec;
        vector<int> v_insertion = vec;
        vector<int> v_selection = vec;
        vector<int> v_sort = vec;

        tc.tic();
        bubble_sort(v_bubble);
        double t_bubble = tc.toc();
        
        tc.tic();
        insertion_sort(v_insertion);
        double t_insertion = tc.toc();

        tc.tic();
        selection_sort(v_selection);
        double t_selection = tc.toc();

        tc.tic();
        sort(v_sort.begin(), v_sort.end());
        double t_sort = tc.toc();

        cout << "Dimensione: " << i << "\n";
        cout << "t_bubble: " << t_bubble << "\n";
        cout << "t_insertion: " << t_insertion << "\n"; 
        cout << "t_selection: " << t_selection << "\n"; 
        cout << "t_sort: " << t_sort << "\n\n";      
    }
    return 0;
}