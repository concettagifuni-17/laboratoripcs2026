#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main() {
    static const int N = 5;
    double arr[N] = {6.5, 7.4, 3.2, 1.3, 2.4};

    bool ordinato = false;
    
    while (!ordinato) {
        ordinato = true; //suppongo sia ordinato
        //devo usare N-1 perchè l'elemento N non esiste ma facendo i+1 arrivo a N e mi stampa spazzatura  
        for (int i=0; i <N -1; i++) {
            if (arr[i] > arr[i+1]) {
                double maggiore = arr[i]; // devo dichiarare una variabile con il valore del primo elemento altriemneti nello scambio mi scmabia due volte lo stesso valore
                arr[i] = arr[i+1];
                arr[i+1] = maggiore;
                ordinato = false;
            }
        }
    }
    
    //per stampare tutto l'array faccio un for
    for (int i=0; i < N; i++) {
        cout << arr[i] << endl;
    }

    return 0;
}