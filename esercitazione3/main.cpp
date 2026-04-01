#include <iostream>
#include "rational.hpp"
using namespace std;

int main(void) {
    rational<int> r(3,6);
    cout << "r = " << r << "\n";
    cout << "num(r) = " << r.num() << ", den(r) = " << r.den() << "\n\n";

    rational<int> pos_inf(5,0);
    rational<int> neg_inf(-3,0);
    rational<int> nan(0,0);

    cout << "Casi speciali:\n";
    cout << "pos_inf = " << pos_inf << "\n";
    cout << "neg_inf = " << neg_inf << "\n";
    cout << "nan = " << nan << "\n\n";

    rational<int> a(1,2);
    rational<int> b(3,4);
    rational<int> z(0,1); 

    cout << "Operatori su razionali interi finiti:\n";
    cout << a << " + " << b << " = " << a + b << "\n";
    cout << a << " - " << b << " = " << a - b << "\n";
    cout << a << " * " << b << " = " << a * b << "\n";
    cout << a << " / " << b << " = " << a / b << "\n\n";

    cout << "Casi speciali negli operatori:\n";
    cout << "Somma:" << "\n";
    cout << a << " + " << nan << " = " << a + nan << "\n";
    cout << pos_inf << " + " << neg_inf << " = " << pos_inf + neg_inf << "\n";
    cout << a << " + " << pos_inf << " = " << a + pos_inf << "\n";
    cout << a << " + " << neg_inf << " = " << a + neg_inf << "\n\n";

    cout << "Differenza:" << "\n";
    cout << a << " - " << nan << " = " << a - nan << "\n";
    cout << pos_inf << " - " << pos_inf << " = " << pos_inf - pos_inf << "\n";
    cout << neg_inf << " - " << pos_inf << " = " << neg_inf - pos_inf << "\n";
    cout << a << " - " << neg_inf << " = " << a - neg_inf << "\n\n";

    cout << "Prodotto:" << "\n";
    cout << a << " * " << nan << " = " << a * nan << "\n";
    cout << pos_inf << " * " << z << " = " << pos_inf * z << "\n";
    cout << neg_inf << " * " << pos_inf << " = " << neg_inf * pos_inf << "\n";
    cout << pos_inf << " * " << b << " = " << pos_inf * b << "\n\n";

    cout << "Rapporto:" << "\n";
    cout << a << " / " << nan << " = " << a/nan << "\n";
    cout << a << " / " << z << " = " << a/z << "\n";
    cout << z << " / " << pos_inf << " = " << z / pos_inf << "\n";
    cout << pos_inf << " / " << pos_inf << " = " << pos_inf / pos_inf << "\n";
    cout << neg_inf << " / " << b << " = " << neg_inf / b <<  "\n";
    cout << a << " / " << pos_inf << " = " << a / pos_inf <<  "\n";

    return 0;
}