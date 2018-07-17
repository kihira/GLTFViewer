cmake_minimum_required(VERSION 3.10)
project(GLTFViewer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

if (VCPKG_TOOLCHAIN OR NOT WIN32)
    option(USE_GLM_PACKAGE "Use find_package for GLM" ON)
    option(USE_GLFW_PACKAGE "Use find_package for GLFW" ON)
endif ()

# Load GLM
if (USE_GLM_PACKAGE)
    find_package(GLM REQUIRED)
else ()
    include_directories(GLTFViewer glm)
endif ()

# Load GLFW
if (USE_GLFW_PACKAGE)
    find_package(GLFW3 3.2 REQUIRED)
else ()
    add_subdirectory(./glfw)
endif ()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Load JSON for Modern C++
include_directories(GLTFViewer include)

# Load GLAD
include_directories(GLTFViewer glad)
list(APPEND LIB_SOURCES glad/glad.c)

# Load ImGui
include_directories(GLTFViewer imgui)
list(APPEND LIB_SOURCES imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_impl_glfw.cpp)

add_executable(GLTFViewer ${LIB_SOURCES} src/main.cpp src/camera/camera.cpp src/camera/camera.h src/vector.hpp src/gltfloader.cpp src/gltfloader.h src/node.cpp src/node.h src/mesh.cpp src/mesh.h src/glhelper.hpp src/asset.cpp src/asset.h src/scene.cpp src/scene.h src/camera/perspective.cpp src/camera/perspective.h src/camera/orthographic.cpp src/camera/orthographic.h src/accessor.h src/bufferview.h)

target_link_libraries(GLTFViewer glfw)