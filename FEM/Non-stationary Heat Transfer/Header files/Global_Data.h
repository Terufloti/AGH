#pragma once
#include <vector>

struct Global_Data {
    int mode;
    std::vector<double> Np; // Points of integration vector
    std::vector<double> W;  // Wages vector

    int num_Elements;
    int num_Nodes;
    double Rmin;
    double Rmax;
    double alfa;
    double T0;
    double t_env;
    double TauMax;
    double C;
    double k;
    double Ro;
    double d_L;

    Global_Data();
    Global_Data(const std::string& fileName);
    Global_Data(int num_Elements, double Rmin, double Rmax, double alfa, double T0, double e_env, double TauMax, double C, double k, double Ro);

    void menu();
    void read_data(const std::string& fileName);
    void print_globals();
};