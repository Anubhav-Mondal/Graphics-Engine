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
#include "vsh.h"

// Model Render Details 
   Model modelDetail("scorpion.obj", Color(255, 255, 0), 25.0f);
// Model modelDetail("sphere.obj", Color(255, 255, 255), 10.0f);
// Model modelDetail;
// Model modelDetail("Skull.obj", WHITE, 500.0f);
// Model modelDetail("dragger.obj",WHITE, 1000.0f);

bool renderWireFrame = false;
bool renderFlatShading = true;
int renderMode = 0;  // 0: Wireframe, 1: Flat Shading, 2: Gouraud Shading

// Light
Vec3d lightDir(0.0f, 0.5f, 1.0f);
Color lightColor(255, 255, 0);
Light light1(lightDir, lightColor, 2.0f);

std::vector<Light> lights= {
    Light(Vec3d(0.5f, -0.5f, 1.0f), WHITE, 1.0f)
};

// Post Processing
bool additiveLighting = false;
Color ambientColor(20, 20, 150); // Ambient light color
float ambientIntensity = 0.5f;   // Ambient light intensity
float exposure = 4.0f;           // exposure value 
float saturation = 0.9f;         // saturation value 
float gammaValue = 1.2f;         // gamma value

// Camera 
Vec3d camera(0.0f, 0.0f, 0.0f);
Vec3d cameraDir(0.0f, 0.0f, 1.0f);
float yaw = 0.0f, pitch = 0.0f; 
Vec3d angle(0.0f, 0.0f, 0.0f);
float offset = modelDetail.minOffset;   // Adjust the `offset` to position the model

// Mouse details (no complete yet)
bool firstMouse = true;
float lastX = 400, lastY = 300;  // Initial mouse position
float sensitivity = 0.01f;

// Keyboard callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float speed = 0.0f;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Movement controls (WASD)
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.z += speed;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.z -= speed;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.x -= speed;        
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera.x += speed;;
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
        renderMode -= 1;
        if (renderMode < 0) { renderMode = 2; }
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        yaw += 2.0f;
        renderMode += 1;
        if (renderMode > 2) { renderMode = 0; }
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        angle.x = 0.0f;
        angle.y = 0.0f;
        angle.z = 0.0f;
    }
}

// Mouse movement callback
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;  // Inverted Y
        lastX = xpos;
        lastY = ypos;

        // Adjust sensitivity for smoother rotation
        float rotationSensitivity = 0.005f;
        
        // Horizontal mouse movement rotates around Y-axis
        angle.y += xoffset * rotationSensitivity;
        
        // Vertical mouse movement rotates around X-axis
        angle.x += yoffset * rotationSensitivity;
        
        float combinedMovement = (abs(xoffset) + abs(yoffset)) * 0.5f;
        if (xoffset > 0 && yoffset > 0) {
            angle.z += combinedMovement * rotationSensitivity * 0.3f; // Clockwise
        } else if (xoffset < 0 && yoffset < 0) {
            angle.z -= combinedMovement * rotationSensitivity * 0.3f; // Counter-clockwise
        }
    } else {
        // Reset firstMouse when button is released
        firstMouse = true;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            // Start tracking mouse movement
            firstMouse = true;
        } else if (action == GLFW_RELEASE) {
            // Stop tracking mouse movement
            firstMouse = true;
        }
    }
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

    std::fill(framebuffer.begin(), framebuffer.end(), 0);  // Clear the framebuffer
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    Mesh model;

    // Load object (.obj) file
    model.loadFromObj(modelDetail.fileName);

    while (!glfwWindowShouldClose(window)) {
        std::fill(framebuffer.begin(), framebuffer.end(), 0);   // Clear Screen
        std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::infinity());
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
        Mat4 rotx, roty, rotz, proj, modelMat;
        // Setting the Matrices
        rotx.setRotationX(angle.x);
        roty.setRotationY(angle.y);
        rotz.setRotationZ(angle.z);
        proj.setProjection(FNEAR, FFAR, FFOVRAD, ASPECT_RATIO);
        modelMat = rotx * roty * rotz;

        //In the rendering loop, consider these modifications:
        for (auto& tri : model.tris) {
            // Transformatioin and Projection
            Triangle triRotated = rotate(tri, modelMat);
            Triangle triProjected = vertexShader(triRotated, offset, proj); 

            Vec3d edge1 = triRotated.p[1] - triRotated.p[0];    
            Vec3d edge2 = triRotated.p[2] - triRotated.p[0];
            Vec3d normal = (edge1.cross(edge2)).normalize();
            
            // Culling
            if (normal.dot(cameraDir) < 0) continue;  // Backface Culling
            if (triProjected.isOutside()) continue;   // Fraustram Culling

            // Wireframe Rendering
            if (renderMode == 0) {
                drawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                            triProjected.p[1].x, triProjected.p[1].y,
                            triProjected.p[2].x, triProjected.p[2].y,
                            RED);
            }
            // Flat Shading Rendering
            if (renderMode == 1){
                // Initialize the Final Color
                Color finalColor = modelDetail.baseColor;
                // Ambient Lighting
                finalColor.ambient(ambientColor, ambientIntensity);

                float accumulatedR = finalColor.r;
                float accumulatedG = finalColor.g;
                float accumulatedB = finalColor.b;

                for (auto& light: lights) {
                    // Intensity Calculation
                    light.lightDir = light.lightDir.normalize();
                    float intensity = std::clamp(normal.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);

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

                // Post Processing
                finalColor.postProcess(saturation, exposure, gammaValue);
    
                // Shading (Flat shading)
                drawTriangleZ(triProjected.p[0].x, triProjected.p[0].y, triProjected.p[0].z,
                              triProjected.p[1].x, triProjected.p[1].y, triProjected.p[1].z,
                              triProjected.p[2].x, triProjected.p[2].y, triProjected.p[2].z,
                              finalColor, true);
            }            
            // Gouraud Shading Rendering
            if (renderMode == 2) {

                Vec3d normal1, normal2, normal3;
                if (tri.normalGiven) {
                    normal1 = triRotated.n[0].normalize();
                    normal2 = triRotated.n[1].normalize();
                    normal3 = triRotated.n[2].normalize();
                } else {
                    normal1 = normal;
                    normal2 = normal;
                    normal3 = normal;
                }
                // Initialize the Final Color
                Color finalColor1 = modelDetail.baseColor;
                Color finalColor2 = modelDetail.baseColor;
                Color finalColor3 = modelDetail.baseColor;
                // Ambient Lighting
                finalColor1.ambient(ambientColor, ambientIntensity);
                finalColor2.ambient(ambientColor, ambientIntensity);
                finalColor3.ambient(ambientColor, ambientIntensity);

                float accumulatedR1 = finalColor1.r;
                float accumulatedG1 = finalColor1.g;
                float accumulatedB1 = finalColor1.b;

                float accumulatedR2 = finalColor2.r;
                float accumulatedG2 = finalColor2.g;
                float accumulatedB2 = finalColor2.b;

                float accumulatedR3 = finalColor3.r;
                float accumulatedG3 = finalColor3.g;
                float accumulatedB3 = finalColor3.b;

                for (auto& light: lights) {
                    // Intensity Calculation
                    light.lightDir = light.lightDir.normalize();
                    float intensity1 = std::clamp(normal1.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);
                    float intensity2 = std::clamp(normal2.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);
                    float intensity3 = std::clamp(normal3.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);

                    Color lightContribution1;
                    lightContribution1.r = static_cast<unsigned char>(std::clamp((light.lightColor.r * intensity1), 0.0f, 255.0f));
                    lightContribution1.g = static_cast<unsigned char>(std::clamp((light.lightColor.g * intensity1), 0.0f, 255.0f));
                    lightContribution1.b = static_cast<unsigned char>(std::clamp((light.lightColor.b * intensity1), 0.0f, 255.0f));

                    Color lightContribution2;
                    lightContribution2.r = static_cast<unsigned char>(std::clamp((light.lightColor.r * intensity2), 0.0f, 255.0f));
                    lightContribution2.g = static_cast<unsigned char>(std::clamp((light.lightColor.g * intensity2), 0.0f, 255.0f));
                    lightContribution2.b = static_cast<unsigned char>(std::clamp((light.lightColor.b * intensity2), 0.0f, 255.0f));

                    Color lightContribution3;
                    lightContribution3.r = static_cast<unsigned char>(std::clamp((light.lightColor.r * intensity3), 0.0f, 255.0f));
                    lightContribution3.g = static_cast<unsigned char>(std::clamp((light.lightColor.g * intensity3), 0.0f, 255.0f));
                    lightContribution3.b = static_cast<unsigned char>(std::clamp((light.lightColor.b * intensity3), 0.0f, 255.0f));
        
                    // Lighting Models
                    if (additiveLighting) {
                        accumulatedR1 = std::clamp(accumulatedR1 + lightContribution1.r, 0.0f, 255.0f);
                        accumulatedG1 = std::clamp(accumulatedG1 + lightContribution1.g, 0.0f, 255.0f);
                        accumulatedB1 = std::clamp(accumulatedB1 + lightContribution1.b, 0.0f, 255.0f);

                        accumulatedR2 = std::clamp(accumulatedR2 + lightContribution2.r, 0.0f, 255.0f);
                        accumulatedG2 = std::clamp(accumulatedG2 + lightContribution2.g, 0.0f, 255.0f);
                        accumulatedB2 = std::clamp(accumulatedB2 + lightContribution2.b, 0.0f, 255.0f);

                        accumulatedR3 = std::clamp(accumulatedR3 + lightContribution3.r, 0.0f, 255.0f);
                        accumulatedG3 = std::clamp(accumulatedG3 + lightContribution3.g, 0.0f, 255.0f);
                        accumulatedB3 = std::clamp(accumulatedB3 + lightContribution3.b, 0.0f, 255.0f);

                    } else {
                        accumulatedR1 = std::clamp((accumulatedR1 * lightContribution1.r) / 255.0f, 0.0f, 255.0f);
                        accumulatedG1 = std::clamp((accumulatedG1 * lightContribution1.g) / 255.0f, 0.0f, 255.0f);
                        accumulatedB1 = std::clamp((accumulatedB1 * lightContribution1.b) / 255.0f, 0.0f, 255.0f);

                        accumulatedR2 = std::clamp((accumulatedR2 * lightContribution2.r) / 255.0f, 0.0f, 255.0f);
                        accumulatedG2 = std::clamp((accumulatedG2 * lightContribution2.g) / 255.0f, 0.0f, 255.0f);
                        accumulatedB2 = std::clamp((accumulatedB2 * lightContribution2.b) / 255.0f, 0.0f, 255.0f);

                        accumulatedR3 = std::clamp((accumulatedR3 * lightContribution3.r) / 255.0f, 0.0f, 255.0f);
                        accumulatedG3 = std::clamp((accumulatedG3 * lightContribution3.g) / 255.0f, 0.0f, 255.0f);
                        accumulatedB3 = std::clamp((accumulatedB3 * lightContribution3.b) / 255.0f, 0.0f, 255.0f);
                    }
                }
                
                finalColor1.r = static_cast<unsigned char>(accumulatedR1);
                finalColor1.g = static_cast<unsigned char>(accumulatedG1);
                finalColor1.b = static_cast<unsigned char>(accumulatedB1);
                
                finalColor2.r = static_cast<unsigned char>(accumulatedR2);
                finalColor2.g = static_cast<unsigned char>(accumulatedG2);
                finalColor2.b = static_cast<unsigned char>(accumulatedB2);

                finalColor3.r = static_cast<unsigned char>(accumulatedR3);
                finalColor3.g = static_cast<unsigned char>(accumulatedG3);
                finalColor3.b = static_cast<unsigned char>(accumulatedB3);

                //Post Processing
                finalColor1.postProcess(saturation, exposure, gammaValue);
                finalColor2.postProcess(saturation, exposure, gammaValue);
                finalColor3.postProcess(saturation, exposure, gammaValue);
    
                // Shading (Gouraud shading)
                drawTriangleGradientZ(triProjected.p[0].x, triProjected.p[0].y, triProjected.p[0].z, finalColor1,
                                     triProjected.p[1].x, triProjected.p[1].y, triProjected.p[1].z, finalColor2,
                                     triProjected.p[2].x, triProjected.p[2].y, triProjected.p[2].z, finalColor3);
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
