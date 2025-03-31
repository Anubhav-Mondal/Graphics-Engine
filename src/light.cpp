#include "light.h"

void Light::print() {
    std::cout << "Light Direction: " << lightDir.x << ", " << lightDir.y << ", " << lightDir.z << std::endl;
    std::cout << "Light Color: RGB(" << (int)lightColor.r << ", " << (int)lightColor.g << ", " << (int)lightColor.b << ")" << std::endl;
}