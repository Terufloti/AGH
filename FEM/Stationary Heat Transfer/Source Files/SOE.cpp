#include "SOE.h"

SOE::SOE(const Grid& grid) {
    H_Global.resize(grid.globals.num_Nodes, grid.globals.num_Nodes); H_Global.setZero();
    P_Global.resize(grid.globals.num_Nodes, 1); P_Global.setZero();
    t.resize(grid.globals.num_Nodes, 1); t.setZero();
    for (int i = 0; i < grid.globals.num_Elements; i++) {
        int j = i + 1;
        auto local_H = grid.elements.at(i).H_Local;
        H_Global(i, i) += local_H(0, 0);
        H_Global(i, j) += local_H(0, 1);
        H_Global(j, i) += local_H(1, 0);
        H_Global(j, j) += local_H(1, 1);
    }

    for (int i = 0; i < grid.globals.num_Elements; i++) {
        int j = i + 1;
        auto local_P = grid.elements.at(i).P_Local;
        P_Global(i, 0) += local_P(0, 0);
        P_Global(j, 0) += local_P(1, 0);
    }
}

void SOE::solve() {
    std::cout << "Calculating temperatures vector." << std::endl << std::endl;
    t = H_Global.fullPivLu().solve(-P_Global);
}

void SOE::print_global_H() {
    std::cout << "Global H matrix: " << std::endl;
    for (int i = 0; i < H_Global.rows(); i++) {
        std::cout << "|\t";
        for (int j = 0; j < H_Global.cols(); j++) {
            std::cout << H_Global(i, j) << " ";
        }
        std::cout << "\t|" << std::endl;
    }
}

void SOE::print_global_P() {
    std::cout << "Global P vector: " << std::endl;
    for (int i = 0; i < P_Global.rows(); i++) {
        std::cout << "| ";
        for (int j = 0; j < P_Global.cols(); j++) {
            std::cout << P_Global(i, j) << " ";
        }
        std::cout << "\t|" << std::endl;
    }
}

void SOE::print_t() {
    std::cout << "Temperature vector: " << std::endl;
    for (int i = 0; i < t.rows(); i++) {
        std::cout << "| ";
        for (int j = 0; j < t.cols(); j++) {
            std::cout << t(i, j) << " ";
        }
        std::cout << "\t|" << std::endl;
    }
}