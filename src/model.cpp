#include "model.h"
#include <iostream>

void Model::print() {
    std::cout << "File Name: " << fileName << std::endl;
    std::cout << "Base ";
    baseColor.print();
    std::cout << "Minimum Offsete can be: " << minOffset << std::endl;
}