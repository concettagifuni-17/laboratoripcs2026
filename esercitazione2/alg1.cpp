#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main() {
    static const int N = 5;
    double arr[N] = {1.3, 2.4, 3.2, 6.5, 7.4};

    double minimo = arr[0];
    double massimo = arr[0];

    for (int i = 1; i < N; i++) {
        if (arr[i] < minimo) {
            minimo = arr[i];
        }
        if (arr[i] > massimo) {
            massimo = arr[i];
        }
    }

    double somma = 0.0;
    for (int i = 0; i < N; i++) {
        somma = somma + arr[i];
    }
    double media = somma/N;

    double var = 0.0;
    for (int i = 0; i < N; i++) {
        var = var + 1.0/N * pow(arr[i] - media, 2);    
    }
    double sd = sqrt(var);

    cout << "Il minimo è: " << minimo << endl;
    cout << "Il massimo è: " << massimo << endl;
    cout << "La media è: " << media << endl;
    cout << "La deviazione standard è: " << sd << endl;

    return 0;
}