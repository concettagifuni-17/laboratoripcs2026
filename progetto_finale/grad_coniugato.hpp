#pragma once

#include <iostream>
#include <Eigen/Dense>

Eigen::VectorXd conjugate_gradient(
    const Eigen::MatrixXd& A,
    const Eigen::VectorXd& b,
    const Eigen::VectorXd& x0,
    const double res_tol,
    const unsigned int ite_max,
    unsigned int& ite,
    bool& converged
) {
    converged = false;
    ite = 0;
    Eigen::VectorXd x = x0;
    // residuo iniziale r0 = b - A*x0
    Eigen::VectorXd res = b - A * x;
    // direzione iniziale p0 = r0
    Eigen::VectorXd p = res;

    const double res_norm_0 = res.norm();
    // Se x0 è già soluzione
    if (res_norm_0 == 0.0) {
        converged = true;
        return x;
    }
    while (ite < ite_max && res.norm() > res_tol * res_norm_0) {
        Eigen::VectorXd Ap = A * p;

        // denominatore p_k^T A p_k
        const double den = (p.transpose() * Ap).value();

        if (den > -1.0e-30 && den < 1.0e-30) {
            std::cerr << "Errore: denominatore quasi nullo nel gradiente coniugato.\n";
            return x;
        }

        // alpha_k = (p_k^T r_k) / (p_k^T A p_k)
        const double alpha_k = ((p.transpose() * res) / (p.transpose() * Ap)).value();

        // x_{k+1} = x_k + alpha_k p_k
        x = x + alpha_k * p;

        // r_{k+1} = b - A*x_{k+1}
        Eigen::VectorXd res_new = b - A * x;

        // beta_k = (p_k^T A r_{k+1}) / (p_k^T A p_k)
        const double beta_k = ((p.transpose() * A * res_new) / (p.transpose() * Ap)).value();

        // p_{k+1} = r_{k+1} - beta_k p_k
        p = res_new - beta_k * p;

        res = res_new;
        ite = ite + 1;
    }
    if (res.norm() <= res_tol * res_norm_0) {
        converged = true;
    } else {
        std::cerr << "Errore: il gradiente coniugato non e' arrivato a convergenza.\n";
    }
    return x;
}