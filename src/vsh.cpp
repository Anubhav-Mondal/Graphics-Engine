#include "vsh.h"

Triangle vertexShader(const Triangle& triRotated, const float offset, const Mat4& matProj) {

    Triangle triProjected, triTranslated;

    // Translate away from camera
    triTranslated = triRotated;
    triTranslated.p[0].z += offset;  
    triTranslated.p[1].z += offset;
    triTranslated.p[2].z += offset;

    // Projection
    triProjected.p[0] = matProj * triTranslated.p[0];
    triProjected.p[1] = matProj * triTranslated.p[1];
    triProjected.p[2] = matProj * triTranslated.p[2];

    // Normalize and scale
    triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
    triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
    triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

    triProjected.p[0].x *= 0.5f * (float)SCREEN_WIDTH;
    triProjected.p[0].y *= 0.5f * (float)SCREEN_HEIGHT;
    triProjected.p[1].x *= 0.5f * (float)SCREEN_WIDTH;
    triProjected.p[1].y *= 0.5f * (float)SCREEN_HEIGHT;
    triProjected.p[2].x *= 0.5f * (float)SCREEN_WIDTH;
    triProjected.p[2].y *= 0.5f * (float)SCREEN_HEIGHT;

    return triProjected;
}