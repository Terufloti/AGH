#include "Grid.h"

// I assume same material for every element in this constructor
Grid::Grid(const Global_Data& globals, double k, double S) : globals(globals) {
    std::cout << "Creating grid elements." << std::endl;
    elements.reserve(globals.num_Elements);
    for (int i = 0; i < globals.num_Elements; i++) {
        if (i == 0) {
            elements.emplace_back(globals, k, globals.d_L, S, 1);
        }
        else if (i == globals.num_Elements - 1) {
            elements.emplace_back(globals, k, globals.d_L, S, 2);
        }
        else if (globals.num_Elements == 1) {
            elements.emplace_back(globals, k, globals.d_L, S, 3);
        }
        else {
            elements.emplace_back(globals, k, globals.d_L, S, 0);
        }
    }
    std::cout << "Size of Grid.elements = " << elements.size() << "." << std::endl << std::endl;
}
// This constructor needs already created vector of elements with same type
Grid::Grid(const Global_Data& globals, std::vector<Element>& elements) : globals(globals), elements(elements) {}