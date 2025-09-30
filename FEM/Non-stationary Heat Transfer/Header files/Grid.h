#pragma once
#include <iostream>
#include <vector>
#include "Global_Data.h"
#include "Element.h"

struct Grid {
	const Global_Data& globals;
	std::vector<Element> elements;

	Grid(const Global_Data& globals, double k, double S);
	Grid(const Global_Data& globals, std::vector<Element>& elements);
};