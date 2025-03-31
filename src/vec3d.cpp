#include "vec3d.h"

// Addition
Vec3d Vec3d::operator+ (const Vec3d& v) const {
    return Vec3d(x + v.x, y + v.y, z + v.z);
}

// Subtraction
Vec3d Vec3d::operator- (const Vec3d& v) const {
    return Vec3d(x - v.x, y - v.y, z - v.z);
}

// Scalar multiplication
Vec3d Vec3d::operator* (float scalar) const {
    return Vec3d(x * scalar, y * scalar, z * scalar);
}
Vec3d Vec3d::operator/ (float scalar) const {
    return Vec3d(x / scalar, y / scalar, z / scalar);
}

// Dot product
float Vec3d::dot(const Vec3d& v) const {
    return x*v.x + y*v.y + z*v.z;
}

// Cross product
Vec3d Vec3d::cross(const Vec3d& v) const {
    return Vec3d(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

// Print (for debugging log)
void Vec3d::print() const {
    std::cout << "Vec3d(" << x << ", " << y << ", " << z << ")\n";
}

// Normalize
Vec3d Vec3d::normalize() const {
    float length = sqrt(x*x + y*y + z*z);
    return Vec3d(x / length, y / length, z / length);
}

// Length
float Vec3d::length() const {
    return sqrt(x*x + y*y + z*z);
}