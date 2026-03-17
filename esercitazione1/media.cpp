#include <iostream>
#include <fstream>
#include <string> 
 
using namespace std;

int main(int argc, const char * argv[]) {

    if (argc<2) {
        cerr << "Errore: nome del file non specificato" << endl;
        return 1;
    }
   
    fstream ifs(argv[1]);

    if (!ifs.is_open()) {
        cerr <<"Errore: impossibile aprire il file correttamente" << endl;
        return 1;
    }

    string location;
    double t1, t2, t3, t4;
    double ntemp = 4.0;

    while (ifs >> location >> t1 >> t2 >> t3 >> t4) {
        double media = (t1 + t2 +t3 + t4)/ ntemp;
        cout << location;
        
        for (int i =0; i < 12 - location.size(); i++) {
            cout << " ";
        }
        cout << media <<endl;
    }

    return 0;
}