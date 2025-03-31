#ifndef VEC3D_H
#define VEC3D_H

#include <cmath>
#include <iostream>

struct Vec3d {
    float x, y, z;
    float w = 1;

    Vec3d (float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3d() : x(0), y(0), z(0) {}

    Vec3d operator+ (const Vec3d& v) const;
    Vec3d operator-(const Vec3d& v) const;
    Vec3d operator*(float scalar) const;
    Vec3d operator/(float scalar) const;

    float dot(const Vec3d& v) const;
    Vec3d cross(const Vec3d& v) const;
    Vec3d normalize() const;
    float length() const;

    void print() const;
};

#endif