#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3d.h"

struct Triangle {
    Vec3d p[3];

    // Constructors
    Triangle();
    Triangle(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3);

    // Operators
    Triangle operator+ (const Triangle& tri) const;
    Triangle operator- (const Triangle& tri) const;

    // Methods
    bool isOutside();
    float averageDepth();
    void print();
};

#endif