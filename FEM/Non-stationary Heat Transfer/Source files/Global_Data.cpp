#include <iostream>
#include <fstream>
#include <string>
#include "Global_Data.h"

Global_Data::Global_Data() : num_Elements(0), num_Nodes(1), Rmin(0), Rmax(0), alfa(0), T0(0), t_env(0), TauMax(0), C(0), k(0), Ro(0), mode(0){
    std::cout << "Created empty Global_Data object!" << std::endl;
}

Global_Data::Global_Data(const std::string& fileName) {
    read_data(fileName);
    std::cout << "Created Global_Data object with data from file!" << std::endl;
    num_Nodes = num_Elements + 1;
    d_L = (Rmax - Rmin) / num_Elements;
    menu();
    print_globals();
}

Global_Data::Global_Data(int num_Elements, double Rmin, double Rmax, double alfa, double T0, double e_env, double TauMax, double C, double k, double Ro):
num_Elements(num_Elements), Rmin(Rmin), Rmax(Rmax), alfa(alfa), T0(T0), t_env(t_env), TauMax(TauMax), C(C), k(k), Ro(Ro){
    menu();
    num_Nodes = num_Elements + 1;
    d_L = (Rmax - Rmin) / num_Elements;
    print_globals();
}

void Global_Data::menu() {
    std::cout << "How many integration points should the integration scheme have (2 or 3): ";
    std::cin >> mode;
    std::cout << std::endl;

    if (mode == 2) {
        Np = { -0.57735, 0.57735 };
        W = { 1.0, 1.0 };
    }
    else if (mode == 3) {
        Np = { -0.77459, 0.0, 0.77459 };
        W = { 0.555555, 0.8888888, 0.555555 };
    }
    else {
        std::cerr << "Invalid choice. A 2-point Gauss integration scheme will be used." << std::endl;
        mode = 2;
        Np = { -0.57735, 0.57735 };
        W = { 1.0, 1.0 };
    }
}

void Global_Data::read_data(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Couldn't open file " << fileName << ". " << std::endl;
        return;
    }
    std::string key;
    double value;

    while (file >> key >> value) {
        if (key == "nE") {
            num_Elements = value;
        }
        else if (key == "Rmin") {
            Rmin = value;
        }
        else if (key =="Rmax") {
            Rmax = value;
        }
        else if (key == "alfa") {
            alfa = value;
        }
        else if (key == "T0") {
            T0 = value;
        }
        else if (key == "t_env") {
            t_env = value;
        }
        else if (key == "TauMax") {
            TauMax = value;
        }
        else if (key == "C") {
            C = value;
        }
        else if (key == "Ro") {
            Ro = value;
        }
        else if (key == "k") {
            k = value;
        }
        else {
            std::cerr << "Unknown parametr: " << key << std::endl;
        }
    }
}

void Global_Data::print_globals() {
    std::cout << "Global_Data object parameters: " << std::endl;
    std::cout << "Number of elements: " << num_Elements << std::endl;
    std::cout << "Number of nodes: " << num_Nodes << std::endl;
    std::cout << "Rmin: " << Rmin << std::endl;
    std::cout << "Rmax: " << Rmax << std::endl;
    std::cout << "d_L: " << d_L << std::endl;
    std::cout << "alfa: " << alfa << std::endl;
    std::cout << "T0: " << T0 << std::endl;
    std::cout << "t_env: " << t_env << std::endl;
    std::cout << "TauMax: " << TauMax << std::endl;
    std::cout << "C: " << C << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "Ro: " << Ro << std::endl;
    std::cout << "Integration points: " << mode << std::endl;
}