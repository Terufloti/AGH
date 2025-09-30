#pragma once
#include <Eigen/Dense>
#include "Grid.h"

struct SOE {
    Eigen::MatrixXd H_Global;
    Eigen::MatrixXd P_Global;
    Eigen::MatrixXd t;

    SOE(const Grid& grid);
    void solve();
    void simulate(Global_Data& globals, Grid &grid);
    void update_matrixes(Grid &grid);
    void print_global_H();
    void print_global_P();
    void print_t();
};
