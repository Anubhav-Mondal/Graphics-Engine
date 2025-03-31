#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

// Getting Header Files
#include "renderer.h"
#include "vec3d.h"
#include "mat4.h"
#include "triangle.h"
#include "mesh.h"
#include "color.h"
#include "light.h"
#include "model.h"

// Model Render Details 
std::string modelToRender = "scorpion.obj";
Color baseColor (255, 0, 0);
Model modelDetail("scorpion.obj", Color(255, 255, 0), 50.0f);
// Model modelDetail("sphere.obj", Color(255, 255, 255), 50.0f);
// Model modelDetail;
// Model modelDetail("Skull.obj",WHITE, 1000.0f);
// Model modelDetail("girl.obj",WHITE, 500.0f);

bool renderWireFrame = false;
bool renderFlatShading = true;

// Light
Vec3d lightDir(0.0f, 0.0f, 1.0f);
Color lightColor(255, 255, 0);
Light light1(lightDir, lightColor, 0.07f);

std::vector<Light> lights= {
    // Light(Vec3d(-0.5f, -0.5f, 1.0f), Color(255, 0, 100), 1.0f),
    Light(Vec3d(0.5f, -0.5f, 1.0f), Color(100, 0, 255), 10.0f)
};

// Processing
bool additiveLighting = false;
Color ambientColor(20, 20, 150); // Ambient light color
float ambientIntensity = 0.5f;   // Ambient light intensity
float exposure = 4.0f;           // exposure value 
float saturation = 0.9f;         // saturation value 
float gammaValue = 3.2f;         // gamma value

// Camera 
Vec3d camera(0.0f, 0.0f, 0.0f);
Vec3d cameraDir(0.0f, 0.0f, 1.0f);
float yaw = 0.0f, pitch = 0.0f; 
Vec3d angle(0.0f, 0.0f, 0.0f);
float offset = modelDetail.minOffset;   // Adjust the `offset` to position the model

// Mouse details
bool firstMouse = true;
float lastX = 400, lastY = 300;  // Initial mouse position
float sensitivity = 0.01f;

// Keyboard callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float speed = 0.1f;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Movement controls (WASD)
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.z += speed;
        // angle.x += 0.1f;
        // angle.y += 0.1f;
        angle.z += 0.1f;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.z -= speed;
        // angle.x -= 0.1f;
        angle.y -= 0.1f;
        // angle.z -= 0.1f;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.x -= speed;
        light1.lightDir.x += speed;
        light1.lightDir.z += speed;
        
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.x += speed;;
        light1.lightDir.x -= speed;
        light1.lightDir.z -= speed;
    }

    // Rotate with arrow keys
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        pitch += 2.0f;
        offset -= 10.0f;
        if (offset < modelDetail.minOffset) {
            offset = modelDetail.minOffset;
        }
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        pitch -= 2.0f;
        offset += 10.0f;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        yaw -= 2.0f;
        // additiveLighting = true;
        renderFlatShading = true;
        renderWireFrame = false;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        yaw += 2.0f;
        renderFlatShading = false;
        renderWireFrame = true;
        // additiveLighting = false;
    }

    // std::cout << "Camera Position: " << camera.x << ", " << camera.y<< ", " << camera.z << std::endl;
}

// Mouse movement callback
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // Inverted Y (y goes up when you move the mouse down)
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain the pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // std::cout << "Yaw: " << yaw << ", Pitch: " << pitch << std::endl;
}

// Mouse scroll callback (for zooming)
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.z += yoffset * 0.5f;  // Zoom in/out
    // std::cout << "Zoom: " << camera.z << std::endl;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphics Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, keyCallback);         // Keyboard input
    glfwSetCursorPosCallback(window, mouseCallback); // Mouse movement
    glfwSetScrollCallback(window, scrollCallback);   // Mouse scroll

    // Lock the mouse inside the window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::fill(framebuffer.begin(), framebuffer.end(), 0);  // Clear the framebuffer
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    Mesh model;

    // Load object (.obj) file
    model.loadFromObj(modelDetail.fileName);

    while (!glfwWindowShouldClose(window)) {
        std::fill(framebuffer.begin(), framebuffer.end(), 0);   // Clear Screen
        // FPS Counter
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        frameCount++;
        if (elapsed.count() >= 1.0) {  // Every second
            double fps = frameCount / elapsed.count();
            // Update window title with FPS
            std::string title = "Graphics Engine - FPS: " + std::to_string(static_cast<int>(fps));
            glfwSetWindowTitle(window, title.c_str());
    
            frameCount = 0;      // Reset frame count
            lastTime = currentTime;
        }
        
        glfwPollEvents();
        
        // Matrices Initialization
        Mat4 rotx, roty, rotz, proj;
        // Setting the Matrices
        rotx.setRotationX(angle.x);
        roty.setRotationY(angle.y);
        rotz.setRotationZ(angle.z);
        proj.setProjection(FNEAR, FFAR, FFOVRAD, ASPECT_RATIO);


        //In the rendering loop, consider these modifications:
        for (auto& tri : model.tris) {
            Triangle triProjected, triTranslated, triRotatedZ, triRotatedY, triRotatedZX;
            
            // Apply rotations in a specific order (Z, then X, then Y if needed)
            triRotatedZ.p[0] = rotz * tri.p[0];
            triRotatedZ.p[1] = rotz * tri.p[1];
            triRotatedZ.p[2] = rotz * tri.p[2];

            triRotatedY.p[0] = roty * triRotatedZ.p[0];
            triRotatedY.p[2] = roty * triRotatedZ.p[2];
            triRotatedY.p[1] = roty * triRotatedZ.p[1];

            triRotatedZX.p[0] = rotx * triRotatedY.p[0];
            triRotatedZX.p[1] = rotx * triRotatedY.p[1];
            triRotatedZX.p[2] = rotx * triRotatedY.p[2];

            // Translate away from camera
            triTranslated = triRotatedZX;
            triTranslated.p[0].z += offset;  
            triTranslated.p[1].z += offset;
            triTranslated.p[2].z += offset;

            // Projection
            triProjected.p[0] = proj * triTranslated.p[0];
            triProjected.p[1] = proj * triTranslated.p[1];
            triProjected.p[2] = proj * triTranslated.p[2];

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

            Vec3d edge1 = triRotatedZX.p[1] - triRotatedZX.p[0];    
            Vec3d edge2 = triRotatedZX.p[2] - triRotatedZX.p[0];
            Vec3d normal = edge1.cross(edge2);
            
            // Culling
            if (normal.dot(cameraDir) < 0) continue;
            if (triProjected.isOutside()) continue;   // Fraustram Culling

            // Wireframe Rendering
            if (renderWireFrame) {
                drawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                            triProjected.p[1].x, triProjected.p[1].y,
                            triProjected.p[2].x, triProjected.p[2].y,
                            RED);
            }
            // Flat Shading Rendering
            if (renderFlatShading){
                
                // Initialize the Final Color
                Color finalColor = modelDetail.baseColor;
                // Ambient Lighting
                finalColor.ambient(ambientColor, ambientIntensity);

                float accumulatedR = finalColor.r;
                float accumulatedG = finalColor.g;
                float accumulatedB = finalColor.b;

                for (auto& light: lights) {
                    // Intensity Calculation
                    light1.lightDir = light1.lightDir.normalize();
                    float intensity = std::clamp(normal.dot(light1.lightDir) * light.lightIntensity, 0.0f, 1.0f);

                    Color lightContribution;
                    lightContribution.r = static_cast<unsigned char>(std::clamp((light.lightColor.r * intensity), 0.0f, 255.0f));
                    lightContribution.g = static_cast<unsigned char>(std::clamp((light.lightColor.g * intensity), 0.0f, 255.0f));
                    lightContribution.b = static_cast<unsigned char>(std::clamp((light.lightColor.b * intensity), 0.0f, 255.0f));
        
                    // Lighting Models
                    if (additiveLighting) {
                        accumulatedR = std::clamp(accumulatedR + lightContribution.r, 0.0f, 255.0f);
                        accumulatedG = std::clamp(accumulatedG + lightContribution.g, 0.0f, 255.0f);
                        accumulatedB = std::clamp(accumulatedB + lightContribution.b, 0.0f, 255.0f);
                    } else {
                        accumulatedR = std::clamp((accumulatedR * lightContribution.r) / 255.0f, 0.0f, 255.0f);
                        accumulatedG = std::clamp((accumulatedG * lightContribution.g) / 255.0f, 0.0f, 255.0f);
                        accumulatedB = std::clamp((accumulatedB * lightContribution.b) / 255.0f, 0.0f, 255.0f);
                    }
                }
                
                finalColor.r = static_cast<unsigned char>(accumulatedR);
                finalColor.g = static_cast<unsigned char>(accumulatedG);
                finalColor.b = static_cast<unsigned char>(accumulatedB);

                // Color Correction
                finalColor.saturate(saturation); // Saturation
                finalColor.exposure(exposure);   // Exposure
                finalColor.gammaCorrect(gammaValue);  // Gamma Correction
    
                // Shading (Flat shading)
                drawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                            triProjected.p[1].x, triProjected.p[1].y,
                            triProjected.p[2].x, triProjected.p[2].y,
                            finalColor, true);
            }            
        }   
        
        // Render the framebuffer to the window
        glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer.data());
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
