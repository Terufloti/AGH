#include "SOE.h"
#include <fstream>
#include <iomanip>

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
    t = H_Global.fullPivLu().solve(P_Global);
}

void SOE::simulate(Global_Data& globals, Grid &grid) {
    double dt = globals.d_L * globals.d_L / (0.5 * globals.k / (globals.C * globals.Ro));
    //double dt = 0.43684;

    int n_steps = static_cast<int>(globals.TauMax / dt);

    dt = globals.TauMax / n_steps;

    Eigen::VectorXd T_prev(globals.num_Nodes);
    T_prev.setConstant(globals.T0);

    std::cout << "===============Simulation start. ==================" << std::endl;
    std::cout << "Number of time steps: " << n_steps << " , dt = " << dt << std::endl;
    std::cout << std::endl;

    std::string filename = "results_nE_" + std::to_string(globals.num_Elements) + ".txt";
    std::ofstream file(filename);
    file << "Time [s]\tT_inside [C]\tT_outside [C]\n";

    for (int i = 0; i <= n_steps; ++i) {
        double tau = i * dt;
        double t_env_dyn = globals.t_env;

        for (auto& el : grid.elements) el.H_Local.setZero(), el.P_Local.setZero();

        for (int j = 0; j < globals.num_Elements; ++j) {
            int z = j + 1;
            double T1 = T_prev(j);
            double T2 = T_prev(z);
            double r1 = globals.Rmin + j * globals.d_L;
            double r2 = globals.Rmin + (j + 1) * globals.d_L;
            //DEBUG 
            //std::cout << "Time step: " << i << ", El.: " << j << ", T1: " << T1 << ", T2: " << T2 << ", r1: " << r1 << ", r2: " << r2 << std::endl;
            grid.elements[j].composeMatrices(T1, T2, r1, r2, dt, t_env_dyn, (grid.elements[j].flag == 2 || grid.elements[j].flag == 3));
        }

        this->update_matrixes(grid);
        //std::cout << "H: (fragment): \n" << H_Global.block(0, 0, 3, 3) << std::endl;
        //std::cout << "P: (fragment): \n" << P_Global.block(0, 0, 3, 1) << std::endl;
        this->solve();

        if (static_cast<int>(tau) % 10 == 0 || i == n_steps) {
            double T_ins = t(0);
            double T_out = t(t.size() - 1);
            file << std::fixed << std::setprecision(3) << tau << "\t" << T_ins << "\t" << T_out << "\n";
        }

        /*
        if (i % 10 == 0 || i == n_steps) {
            std::cout << "Tau = " << tau << " s" << std::endl;
            this->print_t();
            std::cout << std::endl;
        }
        if (i == n_steps) {
            std::cout << "H_Local (ostatni element): \n" << grid.elements.back().H_Local << std::endl;
            std::cout << "P_Local (ostatni element): \n" << grid.elements.back().P_Local << std::endl;
        }
        */
        T_prev = this->t;
        //std::cout << "T prev debug: "; <- DEBUG
        //for (int d = 0; d < T_prev.size();++d) {
        //    std::cout << T_prev(d) << " ";
        //}
        //std::cout << std::endl;
    }
    std::cout << "===============Simulation end. ==================" << std::endl;
    std::cout << "Results has been saved to file:  " + filename + "." << std::endl;
    file.close();
}

void SOE::update_matrixes(Grid &grid) {
    int size = grid.globals.num_Nodes;
    H_Global.setZero();
    P_Global.setZero();

    for (int i = 0; i < grid.globals.num_Elements;++i) {
        const Element& e = grid.elements[i];
        H_Global(i, i) += e.H_Local(0, 0);
        H_Global(i, i + 1) += e.H_Local(0, 1);
        H_Global(i+1, i) += e.H_Local(1, 0);
        H_Global(i+1, i + 1) += e.H_Local(1, 1);

        P_Global(i, 0) += e.P_Local(0, 0);
        P_Global(i + 1, 0) += e.P_Local(1, 0);
    }
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
    std::cout << "Vector P: " << std::endl;
    for (int i = 0; i < P_Global.rows(); i++) {
        std::cout << "| ";
        for (int j = 0; j < P_Global.cols(); j++) {
            std::cout << P_Global(i, j) << " ";
        }
        std::cout << "\t|" << std::endl;
    }
}

void SOE::print_t() {
    std::cout << "Temperatures vector: " << std::endl;
    for (int i = 0; i < t.rows(); i++) {
        std::cout << "| ";
        for (int j = 0; j < t.cols(); j++) {
            std::cout << t(i, j) << " ";
        }
        std::cout << "\t|" << std::endl;
    }
}