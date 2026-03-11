#include <iostream>

using namespace std;

int main(int argc, const char *argv[]) {
    cout << "Numero parametri: " << argc << "\n";
    for (int i = 0; i <argc; i++) {
        cout << "Parametro " << i << " = " << argv[i] << "\n";
    }
    return 0;
}