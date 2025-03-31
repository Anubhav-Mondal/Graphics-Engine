#include "mesh.h"
#include <fstream>
#include <string> 
#include <vector>   
#include <sstream>
#include <iostream>

Mesh::Mesh() {
    tris = {};  // Initialize with an empty 
}

void Mesh::print(){
    std::cout << "Mesh contains " << tris.size() << " triangles:\n";
    for (size_t i = 0; i < tris.size(); ++i) {
        std::cout << "Triangle " << i + 1 << ":\n";
        tris[i].print();
    }
}

void Mesh::addTriangle(float x1, float y1, float z1, 
                       float x2, float y2, float z2, 
                       float x3, float y3, float z3) {
    Triangle t;
    t.p[0] = {x1, y1, z1};
    t.p[1] = {x2, y2, z2};
    t.p[2] = {x3, y3, z3};
    tris.push_back(t);
}

bool Mesh::loadFromObj(std::string fileName) {
    fileName = "models/" + fileName;
    std::ifstream f(fileName);

    if (!f.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }

    std::vector<Vec3d> verts;  // Cache for vertices

    std::string line;
    while (std::getline(f, line)) {          // Read each line
        std::stringstream s(line);
        char junk;

        if (line[0] == 'v') {                // Vertex line
            Vec3d v;
            s >> junk >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }

        if (line[0] == 'f') {                // Face line
            std::vector<int> faceIndices;

            std::string vertexInfo;
            s >> junk;

            while (s >> vertexInfo) {        // Read face vertices
                std::stringstream vertexStream(vertexInfo);
                std::string indexStr;
                std::getline(vertexStream, indexStr, '/');  // Extract vertex index
                int vIndex = std::stoi(indexStr) - 1;       // Convert to 0-based index

                // Bounds check
                if (vIndex >= 0 && vIndex < verts.size()) {
                    faceIndices.push_back(vIndex);
                } else {
                    std::cerr << "Invalid face index in .obj file!" << std::endl;
                }
            }

            // Triangulate quads
            if (faceIndices.size() == 4) {
                // Quad → Two triangles
                tris.push_back({ verts[faceIndices[0]], verts[faceIndices[1]], verts[faceIndices[2]] });
                tris.push_back({ verts[faceIndices[0]], verts[faceIndices[2]], verts[faceIndices[3]] });
            }
            else if (faceIndices.size() == 3) {
                // Triangle face
                tris.push_back({ verts[faceIndices[0]], verts[faceIndices[1]], verts[faceIndices[2]] });
            }

            // std::cout << "Total Triangle: " << tris.size() << std::endl;
        }
    }

    return true;
}