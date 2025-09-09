# 3D Graphics Engine (Built from Scratch in C++) 

### **About the Project**
This is a **3D graphics engine** built entirely from scratch in **C++** without using external graphics libraries (like OpenGL or DirectX).  
The goal of this project is to **learn 3D computer graphics through practical application** by implementing everything from **rendering pipelines** to **lighting models** manually.  

---

### **Features**

✅ **Wireframe Rendering** → Real-time rendering of wireframe models with smooth performance.  
✅ **Flat Shading** → Per-triangle lighting calculations with efficient shading.
✅ **Gouraud Shading** → Per-vertex lighting with color interpolation across triangles for smoother shading.
✅ **Deapth Buffer/ Z-Buffer** → Accurate hidden surface removal using depth comparisons, ensuring correct visibility of objects.
✅ **OBJ File Support** → Load and render external `.obj` 3D models.  
✅ **Frustum Culling** → Optimized rendering by culling triangles outside the camera’s frustum.  
✅ **Gamma Correction & Color Saturation** → Improved color accuracy and visual quality.  
✅ **Efficient CPU Rendering** → Entire rendering pipeline runs on the **CPU**, with smart optimizations for better performance.  

---

### **Performance Highlights**
- **Efficient Rendering:** Renders `.obj` models with thousands of triangles at **60-80 FPS** even on a low-spec machine.
- **Optimized Culling & Lighting:** FPS boost achieved through smart frustum culling and optimized shading.  
- **Scalable Performance:** On higher-end CPUs, it will perform significantly faster due to better processing capabilities.  

---

### **Screenshots**
**Wireframe Model**  
![Scorpion obj model - Wireframe](<demo/wireframe.png>)

**Flat Shaded Model** 
 ![Scorpion obj model - Flat Shading](<demo/flat-shading.png>)

**Gouraud Shaded Model** 
 ![Scorpion obj model - Gouraud Shading](<demo/gouraud-shading.png>)
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

---
### **Custom Model**
1. Place Your Model:
- Add your custom 3D model (`.obj` file) to the `models/` folder.
2. Include the Model in Your Code:
- Open `main.cpp` and add the following line anywhere before the `main()` function:
  
  ```cpp
  Model modelDetail("YourModelName.obj", baseColor, offset);
  ```
- Replace the parameters with your desired values:
  - `YourModelName.obj`: The name of your model file.
  - `baseColor`: The base color of the model, using `Color(r, g, b)` format.
  - Example:

    ```cpp
    Color(255, 255, 255)  // White  
    Color(255, 0, 0)      // Red
    ```

  - `offset`:  The distance from the camera. You’ll need to tweak this value to avoid performance issues.
      - Guidelines for offset:
         - If all vertices are within the range `[-1, 1]`, use an offset of around `50.0f`.

         - If the range is larger, increase the offset (e.g., `1000.0f`, `2000.0f`, etc.) for proper rendering.

---
### **Controls**
   **Current Controls**:

   >- `0` → Wireframe (WF)
   >- `1` → Flat Shading (FS)   
   >- `2` → Gouraud Shading (GS)
   
   `←` (Left Arrow): Switch to the previous rendering mode (cyclic).  
> Example: GS → FS → WF → GS ...  

   `→` (Right Arrow): Switch to the next rendering mode (cyclic).  
> Example: WF → FS → GS → WF ...

   `↑` (Up Arrow): Decrease the model offset (model moves closer to the camera).

   `↓` (Down Arrow): Increase the model offset (model moves further from the camera).

   `Left Click + Drag Horizontally` : Rotate model around Y-axis.

   `Left Click + Drag Vertically` : Rotate model around X-axis.

   `Left Click + Drag Diagonally` : Adds slight Z-axis rotation.

   `R`: Reset all rotations to default position.

   `esc`: Exit the application.

  ⚠️ More controls coming soon!

---

### **Author**
- *Name:* Anubhav Mondal
- *Github:* https://github.com/Anubhav-Mondal
