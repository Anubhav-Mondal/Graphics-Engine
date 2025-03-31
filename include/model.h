#ifndef MODEL_H
#define MODEL_H

#include "color.h"
#include <iostream>

struct Model {
    std::string fileName;
    Color baseColor;
    float minOffset;

    // Constructor
    Model () : fileName("cube.obj"), baseColor(WHITE), minOffset(50.0f) {}
    Model (std::string fileName, Color baseColor, float minOffset) : fileName(fileName), baseColor(baseColor), minOffset(minOffset) {}

    // Method
    void print();
};

#endif