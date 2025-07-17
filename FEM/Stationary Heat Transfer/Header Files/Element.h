#pragma once
#include <Eigen/Dense>
#include "Global_Data.h"

struct Element {
    const Global_Data& globals;
    Eigen::Matrix<double, 2, 2> H_Local;
    Eigen::Matrix<double, 2, 1> P_Local;
    double k;
    double L;
    double S;

    Element(const Global_Data& globals, double k, double L, double S, int flag);

    void print_H();
};