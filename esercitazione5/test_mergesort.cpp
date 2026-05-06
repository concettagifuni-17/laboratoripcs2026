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
        rf.fill(n, 0, 100);
        int N = n[0];
        vector<int> vec(N);
        rf.fill(vec, -1000, 1000);
        if (vec.size() > 0) {
            merge_sort(vec, 0, vec.size() - 1);
        }
        if (!is_sorted(vec)) {
            return EXIT_FAILURE;
        }
    }
    vector<string> str = {"gioiello", "nuvoloso", "gatto", "ventilatore", "centrifugare", "infermiera", "elettrico", "schermo", "aquile", "spazio"};
    if (str.size() > 0){
        merge_sort(str, 0, str.size() - 1);
    }
    if (!is_sorted(str)) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}