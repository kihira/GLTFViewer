cmake_minimum_required(VERSION 3.10)
project(GLTFViewer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

if (NOT WIN32)
    option(USE_GLM_PACKAGE "Use find_package for GLM" ON)
    option(USE_GLFW_PACKAGE "Use find_package for GLFW" ON)
endif ()

# Load GLM
if (USE_GLM_PACKAGE)
    find_package(GLM REQUIRED)
else ()
    # todo might not work
    add_subdirectory(./glm)
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

# Load nanoVG
include_directories(GLTFViewer ./nanovg/src)

# Load GLAD
include_directories(GLTFViewer ./glad)

add_executable(GLTFViewer src/main.cpp glad/glad.c)

target_link_libraries(GLTFViewer glfw)