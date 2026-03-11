#include <iostream> 
#include <fstream>
using namespace std;

int main(int argc, const char *argv[]) {
    
    if (argc < 2) {
        cout <<"Errore: nome del file non specificato";
        return 1;
    }
    
    fstream ifs(argv[1]);

    if (!ifs.is_open()) {
        cout <<"Errore: impossibile aprire il file";
        return 1;
    }
    
    string location;
    double t1, t2, t3, t4;
    double ntemp = 4.0;

    while (ifs >> location >> t1 >> t2>> t3 >> t4) {
        double media = (t1+t2+t3+t4)/ntemp;
        cout << location;
        int nspazi = 12 - location.size();
        for (int i = 0; i < nspazi; i++) {
            cout << " ";
        }
        cout << media << endl;
    }
 
    return 0;
}