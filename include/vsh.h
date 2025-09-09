#ifndef VSH_H
#define VSH_H

#include "triangle.h"
#include "mat4.h"
#include "renderer.h"

inline Triangle rotate(const Triangle& tri, const Mat4& rotMat) {
    Triangle rotatedTri;
    
    rotatedTri.p[0] = rotMat * tri.p[0];
    rotatedTri.p[1] = rotMat * tri.p[1];
    rotatedTri.p[2] = rotMat * tri.p[2];

    rotatedTri.n[0] = rotMat * tri.n[0];
    rotatedTri.n[1] = rotMat * tri.n[1];
    rotatedTri.n[2] = rotMat * tri.n[2];

    return rotatedTri;
}

Triangle vertexShader(const Triangle& tri, const float offset, const Mat4& matProj);

#endif