#include <iostream>
#include "Element.h"

Element::Element(const Global_Data& globals, double k, double L, double S, int flag) : globals(globals), k(k), L(L), S(S) {
    double C = S * k / L;

    H_Local(0, 0) = H_Local(1, 1) = C;
    H_Local(0, 1) = H_Local(1, 0) = -1 * C;

    if (flag == 1) { // First element in grid
        P_Local(0, 0) = globals.q * S;
        P_Local(1, 0) = 0;
    }
    else if (flag == 2) { // Last element in grid
        P_Local(0, 0) = 0;
        P_Local(1, 0) = -1 * globals.alfa * globals.t_env * S;

        H_Local(1, 1) += globals.alfa * S;
    }
    else if (flag == 3) { // 1 element object
        P_Local(0, 0) = globals.q * S;
        P_Local(1, 0) = -1 * globals.alfa * globals.t_env * S;

        H_Local(1, 1) += globals.alfa * S;
    }
    else { // Rest of elements
        P_Local(0, 0) = 0;
        P_Local(1, 0) = 0;
    }
}

void Element::print_H() {
    std::cout << "Local H matrix in element: " << std::endl;
    std::cout << "| " << H_Local(0, 0) << " " << H_Local(0, 1) << " |" << std::endl;
    std::cout << "| " << H_Local(1, 0) << " " << H_Local(1, 1) << " |" << std::endl;
}