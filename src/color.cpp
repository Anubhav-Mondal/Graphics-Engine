#include "color.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// Invert the Color (eg- RGB(255, 0, 0) becomes RGB(0, 255, 255))
void Color::invert() {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
}

// Saturate the Color
void Color::saturate(float saturation) {
    float gray = 0.3f * r + 0.59f * g + 0.11f * b;

    r = static_cast<unsigned char>(std::clamp(gray + (r - gray) * saturation, 0.0f, 255.0f));
    g = static_cast<unsigned char>(std::clamp(gray + (g - gray) * saturation, 0.0f, 255.0f));
    b = static_cast<unsigned char>(std::clamp(gray + (b - gray) * saturation, 0.0f, 255.0f));
}

// Apply Exposure to the Color
void Color::exposure(float exposure) {
    r = static_cast<unsigned char>(std::clamp(static_cast<float>(255.0 * (1.0 - exp(-r * exposure / 255.0f))), 0.0f, 255.0f));
    g = static_cast<unsigned char>(std::clamp(static_cast<float>(255.0 * (1.0 - exp(-g * exposure / 255.0f))), 0.0f, 255.0f));
    b = static_cast<unsigned char>(std::clamp(static_cast<float>(255.0 * (1.0 - exp(-b * exposure / 255.0f))), 0.0f, 255.0f));
}

// Ambient Light
void Color::ambient(Color ambientColor, float ambientIntensity) {
    auto clampColor = [](float value) -> unsigned char {
        return static_cast<unsigned char>(std::clamp(value, 0.0f, 255.0f));
    };

    r = clampColor(r * (1 - ambientIntensity) + ambientColor.r * ambientIntensity);
    g = clampColor(g * (1 - ambientIntensity) + ambientColor.g * ambientIntensity);
    b = clampColor(b * (1 - ambientIntensity) + ambientColor.b * ambientIntensity);
}

// Gamma Correction
void Color::gammaCorrect(float gammaValue) {
    float invGamma = 1.0f / gammaValue;

    r = static_cast<unsigned char>(std::clamp(255.0f * std::pow(r / 255.0f, invGamma), 0.0f, 255.0f));
    g = static_cast<unsigned char>(std::clamp(255.0f * std::pow(g / 255.0f, invGamma), 0.0f, 255.0f));
    b = static_cast<unsigned char>(std::clamp(255.0f * std::pow(b / 255.0f, invGamma), 0.0f, 255.0f));
}

// Post Processing (Saturation, Exposure, Gamma Correction)
void Color::postProcess(float saturationValue, float exposureValue, float gammaValue) {
    saturate(saturationValue);
    exposure(exposureValue);
    gammaCorrect(gammaValue);
}

// Clamp the Color
void Color::clamp() {
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
}

// Print the Color
void Color::print() {
    std::cout << "Color: (" << (int)r << ", " << (int)g << ", " << (int)b << ")  [RGB]" << std::endl;
}