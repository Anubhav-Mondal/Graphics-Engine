cmake_minimum_required(VERSION 3.10)
project(MyProject)

include_directories(include)

# Create executable
add_executable(engine src/main.cpp src/renderer.cpp src/vec3d.cpp src/mat4.cpp src/triangle.cpp src/mesh.cpp src/color.cpp src/light.cpp src/model.cpp)

# Link libraries to the correct target
target_link_libraries(engine glfw3 glew32 glu32 opengl32 winmm gdi32 user32 kernel32)
target_include_directories(engine PRIVATE /mingw64/include)
