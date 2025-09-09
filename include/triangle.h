#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3d.h"

struct Triangle {
    Vec3d p[3];
    Vec3d n[3];
    bool normalGiven = false;

    // Constructors
    Triangle();
    Triangle(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3);
    Triangle(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3, const Vec3d& n1, const Vec3d& n2, const Vec3d& n3);

    // Operators
    Triangle operator+ (const Triangle& tri) const;
    Triangle operator- (const Triangle& tri) const;

    // Methods
    bool isOutside();
    float averageDepth();
    Vec3d getCentroid();
    void print();
};

#endif