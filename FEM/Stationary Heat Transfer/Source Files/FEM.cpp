#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "Global_Data.h"
#include "Element.h"
#include "Grid.h"
#include "SOE.h"

int main()
{
    /// Global_Data(double num_Elements, double alfa, double LG, double q, double t_env) <- constructor of Global_Data element
    /// Grid(const Global_Data &globals, double k, double L, double S) <- constructor Grid element
    /// solution constructor needs instance of grid element!


    Global_Data zad1(4, 10, 5, -150, 400);
    Grid grid(zad1, 50, 2);
    SOE solution1(grid);
    solution1.print_global_H();
    solution1.solve();
    solution1.print_t();

}
