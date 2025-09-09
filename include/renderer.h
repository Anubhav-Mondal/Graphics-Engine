#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <cmath>
#include "color.h"

// constants for screen height and width
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;
const int HALF_WIDTH = SCREEN_WIDTH / 2;
const int HALF_HEIGHT = SCREEN_HEIGHT / 2;
const float FNEAR = 0.1f;
const float FFAR = 1000.0f;
const float FOV = 90.0f;
const float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT; 
const float FFOVRAD = FOV * (3.14159f / 180.0f);

// framebuffer - expose
extern std::vector<unsigned char> framebuffer;
extern std::vector<float> zBuffer;

//Drawing functions
void setPixel(int x, int y, Color color);
void drawLine(int x1, int y1, int x2, int y2, Color color);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color, bool fill = false);
void drawTriangleGradient(int x1, int y1, Color color1, int x2, int y2, Color color2, int x3, int y3, Color color3);
void drawTriangleZ(int x1, int y1, float z1, int x2, int y2, float z2, int x3, int y3, float z3, Color color, bool fill);
void drawTriangleGradientZ(int x1, int y1, float z1, Color color1, int x2, int y2, float z2, Color color2, int x3, int y3, float z3, Color color3);
void drawCircle(int centerx, int centery, int radius, Color color, bool fill = false);

#endif