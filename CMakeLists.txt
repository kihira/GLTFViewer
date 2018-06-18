cmake_minimum_required(VERSION 3.10)
project(GLTFViewer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(FREEGLUT FALSE) # Set true if running on Apple with freeglut installed via homebrew
# FreeGLUT limits us to OpenGL 2.1 cause Apple

if (MSVC AND NOT $ENV{CLION_IDE})
    add_definitions(-DRESOURCELOC="../../") # Used for fixing issues with loading in VS
else()
    add_definitions(-DRESOURCELOC="")
endif()

# Find packages
find_package(OpenGL REQUIRED)
if (MSVC)
    set(GLUT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/freeglut")
    set(GLEW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/glew")
    set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/glm")
    set(GLUT_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/freeglut.lib")
    set(GLEW_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/glew32.lib")
elseif (MINGW)
    set(GLUT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/freeglut")
    set(GLEW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/glew")
    set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/glm")
    set(GLUT_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/mingw/libfreeglut.a")
    set(GLEW_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/mingw/libglew32.a" "${CMAKE_SOURCE_DIR}/lib/mingw/libglew32mx.a" "${CMAKE_SOURCE_DIR}/lib/mingw/libglew32.dll.a" "${CMAKE_SOURCE_DIR}/lib/mingw/libglew32mx.dll.a")
else()
    if (APPLE AND FREEGLUT)
        set(GLUT_INCLUDE_DIRS "/usr/local/Cellar/freeglut/3.0.0/include")
        set(GLUT_LIBRARIES "/usr/local/Cellar/freeglut/3.0.0/lib/libglut.dylib")
    else()
        find_package(GLUT REQUIRED)
    endif()
    find_package(GLEW REQUIRED)
    find_package(GLM REQUIRED)
endif()

include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
# Note: glew and glm was installed via homebrew making include platform agnostic (compared to opengl/glut)

add_executable(GLTFViewer main.cpp)

# Copy resources across
add_custom_command(TARGET GLTFViewer PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:GLTFViewer>/resources)

# Copy DLL across depending on build system
if (MINGW)
    add_custom_command(TARGET GLTFViewer PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/dll/mingw $<TARGET_FILE_DIR:GLTFViewer>/)
elseif (MSVC)
    add_custom_command(TARGET GLTFViewer PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/dll/msvc $<TARGET_FILE_DIR:GLTFViewer>/)
endif()

target_link_libraries(GLTFViewer ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})