#ifndef COLOR_H
#define COLOR_H

// Color struct to store RGB values inorder of Red, Green, Blue
struct Color
{
    unsigned char r, g, b;  // red, green, blue

    // Constructor
    Color (unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
    Color() : r(0), g(0), b(0) {}

    // Methods
    void saturate(float saturation);
    void exposure(float exposure);
    void ambient(Color ambientColor, float ambientIntensity);
    void gammaCorrect(float gammaValue);
    void postProcess(float saturation, float exposure, float gammaValue);
    void invert();
    void clamp();
    // Printing the color values
    void print();
};

// Constants
const Color RED(255, 0, 0);
const Color GREEN(0, 255, 0);
const Color BLUE(0, 0, 255);
const Color WHITE(255, 255, 255);
const Color BLACK(0, 0, 0);

#endif