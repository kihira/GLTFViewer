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

Camera *camera;
GLuint program;

static const char *vertShader =
        "#version 330\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 proj;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = proj * view * model * vec4(aPos, 1.0);\n"
        "}\n\0";

static const char *fragShader =
        "#version 330\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.f, 0.5f, 0.5f, 1.0f);\n"
        "}\n\0";

static void glfwErrorCallback(int error, const char *desc) {
    std::cerr << "GLFW Error " << error << ": " << desc << std::endl;
}

static void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera->update(width, height);
    glErrorCheck();

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE, glm::value_ptr(camera->projection));
    glErrorCheck();
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static GLuint compileShader(const char *source, GLuint type) {
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
    GLFWwindow *window = glfwCreateWindow(1, 1, "GLTF Viewer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSwapInterval(1);

    // Init OpenGL and Glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

    // Set OpenGL stuff
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

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
    std::string filePath = "Box.glb";
    asset = gltf::LoadAsset(filePath);

    // Load and build shaders
    GLuint vert, frag;
    vert = compileShader(vertShader, GL_VERTEX_SHADER);
    frag = compileShader(fragShader, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);
    glErrorCheck();

    glfwSetWindowSize(window, 1280, 720); // Set window size to trigger framebuffersize callback

    glm::vec3 eyePos(3.f, 3.f, 3.f);
    glm::vec3 targetPos(0.f, 0.f, 0.f);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
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

        if (ImGui::Begin("View")) {
            ImGui::InputFloat3("Eye", reinterpret_cast<float *>(&eyePos));
            ImGui::InputFloat3("Target", reinterpret_cast<float *>(&targetPos));
            ImGui::End();
        }

        glm::mat4 view = glm::lookAt(eyePos, targetPos, vector::UP);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
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