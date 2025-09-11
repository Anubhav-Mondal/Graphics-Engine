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

//======================//
// Model Render Details //
//======================//

 Model modelDetail("scorpion.obj", WHITE, 25.0f);
// Model modelDetail("sphere.obj", WHITE, 10.0f);
// Model modelDetail;
// Model modelDetail("Skull.obj", WHITE, 500.0f);
// Model modelDetail("dragger.obj", WHITE, 1000.0f);
// Model modelDetail("cylinder.obj", WHITE, 50.0f);

//===============//
// Light Details //
//===============//
Vec3d lightDir(0.0f, 0.5f, 1.0f);
Color lightColor(255, 255, 0);
Light light1(lightDir, lightColor, 2.0f);

std::vector<Light> lights= {
    Light(Vec3d(0.5f, 0.0f, 0.0f), RED, 1.0f),
    Light(Vec3d(-0.5f, 0.0f, 0.0f), GREEN, 1.0f)
};

int renderMode = 0;  // 0: Wireframe, 1: Flat Shading, 2: Gouraud Shading

// Post Processing
bool additiveLighting = false;
Color ambientColor(20, 20, 150); // Ambient light color
float ambientIntensity = 0.05f;   // Ambient light intensity
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

    std::fill(framebuffer.begin(), framebuffer.end(), 20);  // Clear the framebuffer
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    Mesh model;

    // Load object (.obj) file
    model.loadFromObj(modelDetail.fileName);

    while (!glfwWindowShouldClose(window)) {
        std::fill(framebuffer.begin(), framebuffer.end(), 20);   // Clear Screen
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
            if (renderMode == 1) {
                // Initialize the Final Color
                Color finalColor;

                float accumulatedR = 0.0;
                float accumulatedG = 0.0;
                float accumulatedB = 0.0;

                float baseR = modelDetail.baseColor.r / 255.0f;
                float baseG = modelDetail.baseColor.g / 255.0f;
                float baseB = modelDetail.baseColor.b / 255.0f;

                for (auto& light: lights) {
                    // Intensity Calculation
                    light.lightDir = light.lightDir.normalize();
                    float intensity = std::clamp(normal.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);

                    float lightR = light.lightColor.r / 255.0f;
                    float lightG = light.lightColor.g / 255.0f;
                    float lightB = light.lightColor.b / 255.0f;

                    accumulatedR += (baseR * lightR * intensity) * 255.0f;
                    accumulatedG += (baseG * lightG * intensity) * 255.0f;
                    accumulatedB += (baseB * lightB * intensity) * 255.0f;
                }

                float ambR = (ambientColor.r / 255.0f) * ambientIntensity * 255.0f;
                float ambG = (ambientColor.g / 255.0f) * ambientIntensity * 255.0f;
                float ambB = (ambientColor.b / 255.0f) * ambientIntensity * 255.0f;

                accumulatedR += ambR; accumulatedG += ambG; accumulatedB += ambB;
                
                finalColor.r = static_cast<unsigned char>(std::clamp(accumulatedR, 0.0f, 255.0f));
                finalColor.g = static_cast<unsigned char>(std::clamp(accumulatedG, 0.0f, 255.0f));
                finalColor.b = static_cast<unsigned char>(std::clamp(accumulatedB, 0.0f, 255.0f));

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
                Color finalColor1, finalColor2, finalColor3;

                float accumulatedR1 = 0.0f, accumulatedG1 = 0.0f, accumulatedB1 = 0.0f;
                float accumulatedR2 = 0.0f, accumulatedG2 = 0.0f, accumulatedB2 = 0.0f;
                float accumulatedR3 = 0.0f, accumulatedG3 = 0.0f, accumulatedB3 = 0.0f;

                for (auto& light: lights) {
                    light.lightDir = light.lightDir.normalize();
                    float intensity1 = std::clamp(normal1.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);
                    float intensity2 = std::clamp(normal2.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);
                    float intensity3 = std::clamp(normal3.dot(light.lightDir) * light.lightIntensity, 0.0f, 1.0f);

                    // Each light contributes: baseColor * lightColor * intensity
                    // Convert to 0-1 range for calculation
                    float baseR = modelDetail.baseColor.r / 255.0f;
                    float baseG = modelDetail.baseColor.g / 255.0f;
                    float baseB = modelDetail.baseColor.b / 255.0f;
                    
                    float lightR = light.lightColor.r / 255.0f;
                    float lightG = light.lightColor.g / 255.0f;
                    float lightB = light.lightColor.b / 255.0f;

                    // Calculate each light's contribution to each vertex
                    accumulatedR1 += (baseR * lightR * intensity1) * 255.0f;
                    accumulatedG1 += (baseG * lightG * intensity1) * 255.0f;
                    accumulatedB1 += (baseB * lightB * intensity1) * 255.0f;

                    accumulatedR2 += (baseR * lightR * intensity2) * 255.0f;
                    accumulatedG2 += (baseG * lightG * intensity2) * 255.0f;
                    accumulatedB2 += (baseB * lightB * intensity2) * 255.0f;

                    accumulatedR3 += (baseR * lightR * intensity3) * 255.0f;
                    accumulatedG3 += (baseG * lightG * intensity3) * 255.0f;
                    accumulatedB3 += (baseB * lightB * intensity3) * 255.0f;
                }

                // Add ambient lighting after all directional lights
                float ambR = (ambientColor.r / 255.0f) * ambientIntensity * 255.0f;
                float ambG = (ambientColor.g / 255.0f) * ambientIntensity * 255.0f;
                float ambB = (ambientColor.b / 255.0f) * ambientIntensity * 255.0f;

                accumulatedR1 += ambR; accumulatedG1 += ambG; accumulatedB1 += ambB;
                accumulatedR2 += ambR; accumulatedG2 += ambG; accumulatedB2 += ambB;
                accumulatedR3 += ambR; accumulatedG3 += ambG; accumulatedB3 += ambB;

                // Clamp final values
                finalColor1.r = static_cast<unsigned char>(std::clamp(accumulatedR1, 0.0f, 255.0f));
                finalColor1.g = static_cast<unsigned char>(std::clamp(accumulatedG1, 0.0f, 255.0f));
                finalColor1.b = static_cast<unsigned char>(std::clamp(accumulatedB1, 0.0f, 255.0f));

                finalColor2.r = static_cast<unsigned char>(std::clamp(accumulatedR2, 0.0f, 255.0f));
                finalColor2.g = static_cast<unsigned char>(std::clamp(accumulatedG2, 0.0f, 255.0f));
                finalColor2.b = static_cast<unsigned char>(std::clamp(accumulatedB2, 0.0f, 255.0f));

                finalColor3.r = static_cast<unsigned char>(std::clamp(accumulatedR3, 0.0f, 255.0f));
                finalColor3.g = static_cast<unsigned char>(std::clamp(accumulatedG3, 0.0f, 255.0f));
                finalColor3.b = static_cast<unsigned char>(std::clamp(accumulatedB3, 0.0f, 255.0f));

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
