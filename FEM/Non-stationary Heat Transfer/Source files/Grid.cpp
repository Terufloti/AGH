#include "Grid.h"

//Zakładam tutaj elementy z jednakowego materiału w tym konstruktorze
Grid::Grid(const Global_Data& globals, double k, double S) : globals(globals) {
    std::cout << "Generating mesh elements." << std::endl;
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
//Ten konstruktor zakłada utworzony wektor elementów jednego typu
Grid::Grid(const Global_Data& globals, std::vector<Element>& elements) : globals(globals), elements(elements) {}