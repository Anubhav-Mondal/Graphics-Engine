# 3D Graphics Engine (Built from Scratch in C++) 

### **About the Project**
This is a **3D graphics engine** built entirely from scratch in **C++** without using external graphics libraries (like OpenGL or DirectX).  
The goal of this project is to **learn 3D computer graphics through practical application** by implementing everything from **rendering pipelines** to **lighting models** manually.  

---

### **Features**

✅ **Wireframe Rendering** → Real-time rendering of wireframe models with smooth performance.  
✅ **Flat Shading** → Per-triangle lighting calculations with efficient shading.  
✅ **OBJ File Support** → Load and render external `.obj` 3D models.  
✅ **Frustum Culling** → Optimized rendering by culling triangles outside the camera’s frustum.  
✅ **Gamma Correction & Color Saturation** → Improved color accuracy and visual quality.  
✅ **Efficient CPU Rendering** → Entire rendering pipeline runs on the **CPU**, with smart optimizations for better performance.  

---

### **Performance Highlights**
- **Efficient Rendering:** Renders `.obj` models with thousands of triangles at **60-80 FPS** even on a low-spec machine.
- **Optimized Culling & Lighting:** FPS boost achieved through smart frustum culling and optimized shading.  
- **Scalable Performance:** On higher-end GPUs, it will perform significantly faster due to better processing capabilities.  

---

### **Screenshots**
**Flat Shaded Model**  
![Scorpion obj model - flatshading](<demo/Screenshot 2025-03-31 134928.png>)

**Wireframe Model** 
 ![Scorpion obj model - wireframe](<demo/Screenshot 2025-03-31 135025.png>)
 ![Scorpion obj model - wireframe - side view](<demo/Screenshot 2025-03-31 135129.png>)
---

### **How It Works**
1. **Vertex Transformation:**  
   - Transforms model vertices using *translation*, *rotation*, and *scaling matrices*.  
2. **Backface Culling:**  
   - Removes triangles facing away from the camera to reduce unnecessary rendering.  
3. **Lighting & Shading:**  
   - Implements *Additive* and *Multiplicative lighting models* with adjustable light intensity and position.  
4. **Frustum Culling:**  
   - Eliminates triangles outside the screen frustum to boost performance.  
5. **Color Correction:**  
   - Applies *gamma correction*, *exposure*, and *saturation* for better visuals.  

---

### **Installation**
1. Clone the repository:  
   ```bash
   git clone https://github.com/Anubhav-Mondal/Graphics-Engine.git
   cd 3d-graphics-engine 
   ```
   > clone the project then go to the project folder.

2. Build the Project:
   ```bash
   cmake -S . -B build
   cmake --build build
   ```

3. (Optional) Clean Build:
   ```bash
   rm -rf build
   cmake -S . -B build
   cmake --build build
   ```

4. Run the Project:
   ```bash
   ./build/engine.exe
   ```
   > The project will build in the `/build` directory.

---

### **Explaination of Commands**
- `cmake -S . -B build` → Generates the build configuration files in the `build` folder.  
- `cmake --build build` → Compiles the project using the build configuration.  
- `./build/engine.exe` → Runs the compiled executable.

---

### **Requirements**
- Make sure you have `CMake` installed.
- Use `MSYS` or `MinGW` for compilation on Window.


### **Controls**
   To be added soon
   (for now only debug controls are added)

### **Author**
- *Name:* Anubhav Mondal
- *Github:* https://github.com/Anubhav-Mondal
