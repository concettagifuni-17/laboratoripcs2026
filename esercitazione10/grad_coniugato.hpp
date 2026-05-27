//funzione che calcola quanto una soluzione è sbagliata tramite il residuo
//si ferma quando il residuo diventa abbastanza piccolo oppure quando raggiunge il numero massimo di iterazioni
//restituisce la soluzione approssimata x
#pragma once

#include <Eigen/Dense>

Eigen::VectorXd conjugate_gradient(
    const Eigen::MatrixXd& A,        //matrice del sistema
    const Eigen::VectorXd& b,        //termino noto 
    const Eigen::VectorXd& x0,       //vettore iniziale arbitrario (soluzione iniziale)
    const double res_tol,            //tolleranza sul residio: decide quanto deve essere precisa la soluzione
    const unsigned int ite_max,      //numero massimo di iterazioni per evitare cicli infiniti
    unsigned int& ite ) {

    Eigen::VectorXd x = x0;
    Eigen::VectorXd res = b - A * x;  //residuo iniziale r0 = b - A*x0 (residuo misura quanto x è lontano dall'essere soluzione del sistema)
    Eigen::VectorXd p = res;          //p = direzione, prima direzione di discesa p0 uguale al residuo iniziale r0

    const double res0_norm_ = res.norm();                          //salvo la norma del residuo iniziale, serve per controllare il residuo relativo durante il ciclo
    ite = 0;
    while (ite < ite_max && res.norm() > res_tol * res0_norm_) {   //continuo il ciclo finchè non ho superato il numero massimo di itrazioni e il residuo non è ancora abbastanza piccolo
        Eigen::VectorXd Ap = A * p;                              
        const double alpha_k = p.dot(res)/ p.dot(Ap);             //alpha_k = (p_k^T r_k) / (p_k^T A p_k)
        //alpha_k = ((p.transpose()*res)/ (p.transpose()*A*p)).value();
        x = x + alpha_k * p;                                     //aggiorno la soluzione: x_{k+1} = x_k + alpha_k * p_k
        Eigen::VectorXd res_new = b - A * x;                     //nuovo residuo: r_{k+1} = b - A*x_{k+1}
        const double beta_k = p.dot(A * res_new) / p.dot(Ap);    //beta_k = (p_k^T A r_{k+1}) / (p_k^T A p_k)
        p = res_new - beta_k * p;                                //p_{k+1} = r_{k+1} - beta_k * p_k aggiorno la direzione di discesa
        res = res_new;
        ite = ite + 1;
    }
    return x;
}