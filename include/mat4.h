#ifndef MAT4_H
#define MAT4_H

#include <array>
#include "vec3d.h"

struct Mat4 {
    std::array<std::array<float, 4>, 4> m;

    Mat4();

    Mat4 operator*(const Mat4& mat) const;
    Vec3d operator*(const Vec3d& v) const;

    // Set as perspective projection matrix
    void setPerspective(float fov, float aspect, float near, float far);

    // Set as translation matrix
    // void setTranslation(float x, float y, float z);

    // Set Rotation around X axis
    void setRotationX(float angle);
    // Set Rotation around Y axis
    void setRotationY(float angle);
    // Set Rotation around Z axis
    void setRotationZ(float angle);
    // Set Projection Matrix
    void setProjection(float near, float far, float fov, float aspect);
    // Set View
    void setView(const Vec3d& eye, const Vec3d& target, const Vec3d& up);

};

#endif 