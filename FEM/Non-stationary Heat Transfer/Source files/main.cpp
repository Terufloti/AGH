#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "Global_Data.h"
#include "Element.h"
#include "Grid.h"
#include "SOE.h"

int main()
{
    Global_Data globals1("dane1.txt");
    //Global_Data globals1("dane2.txt");
    //Global_Data globals1("dane3.txt");
    //Global_Data globals1("dane4.txt");
    Grid siata1(globals1, globals1.k, 1.0);
    SOE solution(siata1);
    solution.simulate(globals1, siata1);
    return 0;
}
