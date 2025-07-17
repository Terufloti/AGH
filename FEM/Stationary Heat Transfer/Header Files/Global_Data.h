#pragma once

struct Global_Data {
    int num_Elements;
    int num_Nodes;
    double alfa;
    double LG;
    double d_L;
    double q;
    double t_env;

    Global_Data();
    Global_Data(int num_Elements, double alfa, double LG, double q, double t_env);
};