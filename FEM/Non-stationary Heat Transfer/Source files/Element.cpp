#include <iostream>
#include "Element.h"

Element::Element(const Global_Data& globals, double k, double L, double S, int flag) : globals(globals), k(k), L(L), S(S), flag(flag){
    H_Local.setZero();
    P_Local.setZero();
}

void Element::print_H() {
    std::cout << "Local H matrix of element: " << std::endl;
    std::cout << "| " << H_Local(0, 0) << " " << H_Local(0, 1) << " |" << std::endl;
    std::cout << "| " << H_Local(1, 0) << " " << H_Local(1, 1) << " |" << std::endl;
}

void Element::composeMatrices(double T1, double T2, double r1, double r2, double dt, double t_env_dyn, bool last) {
    for (int ip = 0; ip < globals.Np.size();++ip) {
        double ksi = globals.Np[ip];
        double w = globals.W[ip];

        double N1 = 0.5 * (1 - ksi);
        double N2 = 0.5 * (1 + ksi);

        double r = N1 * r1 + N2 * r2;

        double T = N1 * T1 + N2 * T2;

        double C_rho = globals.C * globals.Ro;
        double K = k;
        double dR = r2 - r1;

        H_Local(0, 0) += K * r * w / dR + C_rho * dR * r * w * N1 * N1 / dt;
        H_Local(0, 1) += -K * r * w / dR + C_rho * dR * r * w * N1 * N2 / dt;
        H_Local(1, 0) += -K * r * w / dR + C_rho * dR * r * w * N2 * N1 / dt;
        H_Local(1, 1) += K * r * w / dR + C_rho * dR * r * w * N2 * N2 / dt;

        P_Local(0) += C_rho * dR * r * w * N1 * N1 * T1 / dt + C_rho * dR * r * w * N1 * N2 * T2 / dt;
        P_Local(1) += C_rho * dR * r * w * N2 * N1 * T1 / dt + C_rho * dR * r * w * N2 * N2 * T2 / dt;

        if (last) {
            H_Local(1, 1) += 2.0 * globals.alfa * r2;
            P_Local(1) += 2.0 * globals.alfa * r2 * t_env_dyn;
        }
    }
}