#include <iostream>
#include "Global_Data.h"

Global_Data::Global_Data() : num_Elements(0), num_Nodes(1), alfa(0), LG(0), d_L(0), q(0), t_env(0) {
    std::cout << "Utworzono obiekt Global_Data nie zawierajacy danych!" << std::endl;
}

Global_Data::Global_Data(int num_Elements, double alfa, double LG, double q, double t_env) : 
    num_Elements(num_Elements), 
    alfa(alfa), 
    LG(LG), 
    q(q), 
    t_env(t_env) {
        num_Nodes = num_Elements + 1;
        d_L = LG / num_Elements;
        std::cout << "Object Global_Data with following properties has been created: " <<std::endl;
        std::cout << "Number of elements: " << num_Elements << std::endl;
        std::cout << "Number of nodes: " << num_Nodes << std::endl;
        std::cout << "Length of whole object (LG): " << LG << std::endl;
        std::cout << "Delta L: " << d_L << std::endl;
        std::cout << "Alfa: " << alfa << std::endl;
        std::cout << "q: " << q << std::endl;
        std::cout << "Environment temperature: " << t_env << std::endl << std::endl;
}