#include "triangle.h"
#include "vec3d.h"
#include "renderer.h"

Triangle::Triangle(): p{Vec3d(0.0f, 0.0f, 0.0f), Vec3d(0.0f, 0.0f, 0.0f), Vec3d(0.0f, 0.0f, 0.0f)}, normalGiven(false) {}
Triangle::Triangle(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3): p{v1, v2, v3}, normalGiven(false) {}
Triangle::Triangle(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3, const Vec3d& n1, const Vec3d& n2, const Vec3d& n3): p{v1, v2, v3}, n{n1, n2, n3}, normalGiven(true) {}

// Operators
Triangle Triangle::operator+ (const Triangle& tri) const {
    return Triangle(
        p[0]+tri.p[0],
        p[1]+tri.p[1],
        p[2]+tri.p[2]
    );
}

Triangle Triangle::operator- (const Triangle& tri) const {
    return Triangle(
        p[0]-tri.p[0],
        p[1]-tri.p[1],
        p[2]-tri.p[2]
    );
}

// Check if triangle is ouotside of the screen (for culling purpose)
bool Triangle::isOutside() {
    bool outLeft   = true;
    bool outRight  = true;
    bool outTop    = true;
    bool outBottom = true;

    for (int i = 0; i < 3; i++) {
        if (p[i].x >= 0)            outLeft = false;    // At least one vertex inside left bound
        if (p[i].x <= SCREEN_WIDTH) outRight = false;   // At least one vertex inside right bound
        if (p[i].y >= 0)            outTop = false;     // At least one vertex inside top bound
        if (p[i].y <= SCREEN_HEIGHT) outBottom = false; // At least one vertex inside bottom bound
    }

    return (outLeft || outRight || outTop || outBottom);
}

// Calculate the average depth (z)
float Triangle::averageDepth() {
    float z1 = p[0].z;
    float z2 = p[1].z;
    float z3 = p[2].z;

    return (z1 + z2 + z3) / 3.0f;
}

// Calculate the centroid of Triangle
Vec3d Triangle::getCentroid() {
    Vec3d sumOfVertices = p[0] + p[1] + p[2];
    return sumOfVertices/3.0f;
}

// Print
void Triangle::print() {
    std::cout << "Triangle: \n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "  P" << i + 1 << ": (" 
                  << p[i].x << ", " 
                  << p[i].y << ", " 
                  << p[i].z << ")\n";
    }
}
