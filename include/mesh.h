#ifndef MESH_H
#define MESH_H

#include <vector>
#include "triangle.h"

struct Mesh {
    std::vector<Triangle> tris;
    Mesh();
    void print();
    void addTriangle(float x1, float y1, float z1, 
                     float x2, float y2, float z2, 
                     float x3, float y3, float z3);
    bool loadFromObj(std::string fileName);
};

#endif