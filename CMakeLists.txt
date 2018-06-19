cmake_minimum_required(VERSION 3.10)
project(GLTFViewer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

if (MSVC AND NOT $ENV{CLION_IDE})
    add_definitions(-DRESOURCELOC="../../") # Used for fixing issues with loading in VS
else()
    add_definitions(-DRESOURCELOC="")
endif()

# Find packages
if (MSVC OR MINGW)
    # Add GLFW
    add_subdirectory(./glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
else()
    find_package(GLFW3 3.2 REQUIRED)
    find_package(GLM REQUIRED)
endif()

file(GLOB SOURCES )

add_executable(GLTFViewer src/main.cpp src/glad.c)

include_directories(GLTFViewer ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(GLTFViewer glfw)