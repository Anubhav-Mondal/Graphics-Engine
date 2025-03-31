#ifndef LIGHT_H
#define LIGHT_H

#include "vec3d.h"
#include "color.h"

struct Light {
    Vec3d lightDir;       // The direction of the light (going)
    Color lightColor;     // Color of the light
    float lightIntensity; // Intensity of the light

    // Constructor
    Light(Vec3d lightDir, Color lightColor, float lightIntensity) : lightDir(lightDir), lightColor(lightColor), lightIntensity(lightIntensity) {}
    Light(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, float lightIntensity) : lightDir(Vec3d(x, y, z)), lightColor(Color(r, g, b)), lightIntensity(lightIntensity) {}
    Light() : lightDir(Vec3d(0.0f, 0.0f, 1.0f)), lightColor(Color(255, 255, 255)), lightIntensity(1.0f) {}

    // Print
    void print();
};


#endif