#include "mat4.h"
#include <cmath>

// Initialize (as Identifier)
Mat4::Mat4() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = (i == j) ? 1.0f : 0.0f;
}

// Matrix Multiplication
Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }

    return result;
}

// Matrix-Vector Multiplication
Vec3d Mat4::operator*(const Vec3d& v) const {
    float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
    float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
    float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
    float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];

    if (w != 0.0f) {
        x /= w;
        y /= w;
        z /= w;
    }

    return Vec3d(x, y, z);
}


// Set as Perspective Projection Matrix
void Mat4::setPerspective(float fov, float aspect, float zNear, float zFar) {
    float f = 1.0f / tan(fov / 2.0f);

    m[0][0] = f / aspect;
    m[1][1] = f;
    m[2][2] = zFar / (zFar - zNear);
    m[2][3] = 1.0f;
    m[3][2] = (-zFar * zNear) / (zFar - zNear);
    m[3][3] = 0.0f;
}


// Set Rotation around X axis
void Mat4::setRotationX (float angle) {
    float c = cos(angle);
    float s = sin(angle);

    m = {{
        {1, 0,  0, 0},
        {0, c, -s, 0},
        {0, s,  c, 0},
        {0, 0,  0, 1}
    }};
}

// Set Rotation around Y axis
void Mat4::setRotationY(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    m = {{
        {c, 0, s, 0},
        {0, 1, 0, 0},
        {-s, 0, c, 0},
        {0, 0, 0, 1}
    }};
}

// Set Rotation around Z axis
void Mat4::setRotationZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    m = {{
        {c, -s, 0, 0},
        {s,  c, 0, 0},
        {0,  0, 1, 0},
        {0,  0, 0, 1}
    }};
}

// Set as Projection Matrix
void Mat4::setProjection(float near, float far, float fov, float aspect) {
    float f = 1.0f / tan(fov * 0.5f);   // Cotangent of half the FOV
    float q = far / (far - near);

    // Zero out the matrix first
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = 0.0f;

    m[0][0] = f / aspect;
    m[1][1] = f;
    m[2][2] = q;
    m[2][3] = -1.0f;
    m[3][2] = -q * near;
}

void Mat4::setView(const Vec3d& eye, const Vec3d& target, const Vec3d& up) {
    Vec3d zAxis = (eye - target).normalize();   // Camera direction (backward)
    Vec3d xAxis = up.cross(zAxis).normalize();  // Right vector
    Vec3d yAxis = zAxis.cross(xAxis).normalize();  // Up vector

    m[0][0] = xAxis.x;    m[0][1] = yAxis.x;    m[0][2] = zAxis.x;    m[0][3] = 0.0f;
    m[1][0] = xAxis.y;    m[1][1] = yAxis.y;    m[1][2] = zAxis.y;    m[1][3] = 0.0f;
    m[2][0] = xAxis.z;    m[2][1] = yAxis.z;    m[2][2] = zAxis.z;    m[2][3] = 0.0f;

    m[3][0] = -xAxis.dot(eye);
    m[3][1] = -yAxis.dot(eye);
    m[3][2] = -zAxis.dot(eye);
    m[3][3] = 1.0f;
}