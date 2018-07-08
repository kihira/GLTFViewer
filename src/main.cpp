#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "gltfloader.h"
#include "vector.hpp"

static const char* vertShader =
        "#version 330\n"
        "uniform mat4 mViewProj;\n"
        "uniform mat4 mModel;\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "layout (location = 2) in vec3 aTangent;\n"
        "layout (location = 3) in vec2 aTexCoord;\n"
        "layout (location = 4) in vec3 vCol;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = mViewProj * mModel * vec4(aPos, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

static const char* fragShader =
        "#version 330\n"
        "out vec4 FragColor;\n"
        "in vec3 color;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(color, 1.0);\n"
        "}\n";

static void glfwErrorCallback(int error, const char *desc) {
    std::cerr << "GLFW Error " << error << ": " << desc << std::endl;
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static GLuint compileShader(const char* source, GLuint type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint err;
    GLchar errData[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (!err) {
        glGetShaderInfoLog(shader, 512, nullptr, errData);
        std::cerr << "Failed to compile shader " << ": " << errData << std::endl;
    }
    glErrorCheck();

    return shader;
}

int main() {
    Camera *camera;
    Asset *asset;

    glfwSetErrorCallback(glfwErrorCallback);

    // Setup and init GLFW
    std::cout << "GLFW " << glfwGetVersionString() << std::endl;
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(1280, 720, "GLTF Viewer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Init OpenGL and Glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

    // Set OpenGL stuff
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glfwSwapInterval(1);

    // Init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    ImGuiIO &io = ImGui::GetIO();

    // Init camera
    camera = new PerspectiveCamera("Default");

    // Load test asset
    std::string filePath = "BoxTextured.glb";
    asset = gltf::LoadAsset(filePath);

    // Load and build shaders
    GLuint vert, frag, program;
    vert = compileShader(vertShader, GL_VERTEX_SHADER);
    frag = compileShader(fragShader, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    glErrorCheck();

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT

        // Start imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Scenes")) {
            ImGui::MenuItem("Scene 1");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        glUseProgram(program);
        glErrorCheck();

        // Update camera
        camera->update(width, height);
        glm::mat4 vp = glm::lookAt(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), vector::UP);
        vp *= camera->projection;
        glUniformMatrix4fv(glGetUniformLocation(program, "mViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glErrorCheck();

        // Draw asset
        if (asset != nullptr) {
            asset->render();
            glErrorCheck();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}