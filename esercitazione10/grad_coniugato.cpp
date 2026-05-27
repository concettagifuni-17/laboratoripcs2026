//testo la funzione conjugate_gradient()
//Creo la matrice A valida per il gradiente coniugato
//Scelgo una soluzione esatta x_ex = [1,1,1]
//Costruisco b = A*x_ex
//chiamo il mio algoritmo
//controllo se la soluzione trovata è vicina a x_ex
//stampo se il test è superato
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/SVD>

#include "grad_coniugato.hpp"
using namespace std;

//funzione che usiamo per calcolare il numero di condizionamenti della matrice
double condA(const Eigen::MatrixXd& A) {
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
    Eigen::VectorXd singularValuesA = svd.singularValues();
    return singularValuesA.maxCoeff() / singularValuesA.minCoeff();
}
//più condA è grande più il sistema può essere difficile da risolvere con precisione

int main() {
    const double tol = 1.0e-15;   //tolleranza molto piccola per controllare se una matrice è quasi singolare
    const unsigned int n = 3;     //dimensione 3x3 per il test

    //creo una matrice casuale B di dimensione 3x3
    Eigen::MatrixXd B = Eigen::MatrixXd::Random(n,n);

    if (abs(B.determinant()) < tol) {   //controlllo il det di B, se è quasi zero allora B è singolare o quasi e quindi non va bene per il test
        return -1;
    }

    Eigen::MatrixXd A = B.transpose() * B;            // A = B^T*B per ottenere la matrice simmetrica definita positiva 
    Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);  //soluzione esatta nota, vettore di tutti 1
    Eigen::VectorXd b = A * x_ex;                     //costruisco b in modo che la soluzione del sistema Ax = b sia x_ex
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n);    //scelgo il vettore iniziale

    const unsigned int ite_max = 10000;
    const double res_tol = 1.0e-12;

    unsigned int ite =0;
    //chiamo la funzione del gradiente coniugato
    Eigen::VectorXd x = conjugate_gradient(A, b, x0, res_tol, ite_max, ite);
    //calcolo il residuo finale
    Eigen::VectorXd residuo = b - A * x;

    //calcolo l'errore relativo tra la soluzione calcolata e la soluzione esatta (faccio un controllo per evitare la divisione per zero)
    double err_rel = (x_ex.norm() == 0.0) ? (x - x_ex).norm() :
                                            (x - x_ex).norm() / x_ex.norm();

    cout.precision(4);

    cout << "Condizionamento matrice A  : " << condA(A) << "\n";
    cout << "Iterazioni effettuate: " << scientific<< ite << "/"<< ite_max<< "\n";
    cout << "Norma residuo finale: " << scientific<< residuo.norm() << "\n";
    cout << "Errore relativo: " << scientific << err_rel<< "\n";
    cout << "soluzione calcolata"<< "\n";
    cout << scientific<< x << "\n";
    cout << "soluzione esatta"<< "\n";
    cout << scientific<< x_ex << "\n";

    if (err_rel < 1.0e-8) {
        cout << "Test superato" << "\n";
    } else {
        cout << "Test fallito" << "\n";
    }
    return 0;
}