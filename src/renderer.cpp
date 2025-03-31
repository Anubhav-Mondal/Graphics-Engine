#include "renderer.h"
#include "color.h"
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>

// framebuffer to store pixel data 
std::vector<unsigned char> framebuffer(SCREEN_WIDTH * SCREEN_HEIGHT * 4, 0);      // 4 bytes per pixel: RGBA

// Set Pixel Color
void setPixel (int x, int y, Color color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        int index = (y * SCREEN_WIDTH + x) * 4;
        framebuffer[index] = color.r;       // Red
        framebuffer[index + 1] = color.g;   // Green
        framebuffer[index + 2] = color.b;   // Blue
        framebuffer[index + 3] = 255; // Alpha
    }
}

// Draw Line  (Bresenham's line algorithm)
void drawLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        setPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

// Draw Triangle 
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color, bool fill) {
    if (fill) {
        // ✅ Sort vertices by Y-coordinate (top-to-bottom)
        if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
        if (y2 > y3) { std::swap(x2, x3); std::swap(y2, y3); }
        if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }

        // ✅ Pre-calculate area denominator once
        float denom = (float)((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));

        // ✅ Use bounding box for better performance
        int minX = std::min({x1, x2, x3});
        int maxX = std::max({x1, x2, x3});
        int minY = std::min({y1, y2, y3});
        int maxY = std::max({y1, y2, y3});

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                // ✅ Pre-calculate barycentric coordinates
                float alpha = ((float)(y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / denom;
                float beta  = ((float)(y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / denom;
                float gamma = 1.0f - alpha - beta;

                // ✅ Only draw if the point is inside the triangle
                if (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f) {
                    setPixel(x, y, color);
                }
            }
        }
    } else {
        // ✅ Draw outline
        drawLine(x1, y1, x2, y2, color);
        drawLine(x2, y2, x3, y3, color);
        drawLine(x3, y3, x1, y1, color);
    }
}
 
// Draw Circle 
void drawCircle(int centerX, int centerY, int radius, Color color, bool fill) {
    if (fill){
        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                if (x * x + y * y <= radius * radius) {
                    setPixel(centerX + x, centerY + y, color);
                }
            }
        }
    } else {
        int x = radius, y = 0;
        int radiusError = 1 - x;

        while (x >= y) {
            setPixel(centerX + x, centerY + y, color);
            setPixel(centerX - x, centerY + y, color);
            setPixel(centerX + x, centerY - y, color);
            setPixel(centerX - x, centerY - y, color);
            setPixel(centerX + y, centerY + x, color);
            setPixel(centerX - y, centerY + x, color);
            setPixel(centerX + y, centerY - x, color);
            setPixel(centerX - y, centerY - x, color);

            y++;
            if (radiusError < 0) {
                radiusError += 2 * y + 1;
            } else {
                x--;
                radiusError += 2 * (y - x) + 1;
            }
        }
    }
}

