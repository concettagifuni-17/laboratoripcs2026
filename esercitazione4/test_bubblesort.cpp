#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

#include "sorts.hpp"
#include "randfiller.h"

int main() {
    randfiller rf;
    for (int i = 0; i < 100; i++) {
        vector<int> n(1);
        rf.fill(n, 1, 100);
        int N = n[0];
        vector<int> vec(N);
        rf.fill(vec, -1000, 1000);
        bubble_sort(vec);
        if (!is_sorted(vec)) {
            return EXIT_FAILURE;
        }
    }
    vector<string> str = {"gioiello", "nuvoloso", "gatto", "ventilatore", "centrifugare", "infermiera", "elettrico", "schermo", "aquile", "spazio"};
    bubble_sort(str);
    if (!is_sorted(str)) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}