#include <iostream>
using namespace std;

int main() {
    double ad[4] = {0.0, 1.1, 2.2, 3.3};
    float af[8] = {0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
    int ai[3] = {0, 1, 2};

    int x= 1;
    float y= 1.1;
   
    cout << "&ad: " << "{"; 
    for (int i =0; i < 3; i++) {
        cout << &ad[i] << "," << " " ;
    }
    cout << &ad[3]<<"}" << endl;

    cout << "&af: "<< "{";
    for (int i =0; i < 7; i++) {
        cout << &af[i] << ","<< " " ;
    }
    cout << &af[7] <<"}" << endl;

    cout << "&ai: " << "{";
    for (int i =0; i < 2; i++) {
        cout << &ai[i] << "," <<" " ;
    }
    cout << &ai[2] << "}" << endl;

    cout << "&x: " << &x << endl << "&y: " << &y << endl;

    return 0;
}