#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "camera.h"
#include "vector.hpp"

Camera::Camera(std::string name) : name(name) {}

/*
 * Orthographic Camera
 */

OrthographicCamera::OrthographicCamera(std::string name) : Camera(name) {}

void OrthographicCamera::update(float width, float height) {
    //projection = glm::ortho(0.f, xMag, 0.f, yMag);
    projection[0][0] = 1.f / xMag;
    projection[1][1] = 1.f / yMag;
    projection[2][2] = 2.f / (zNear / zFar);
    projection[3][2] = (zFar + zNear) / (zNear / zFar);

    if (ImGui::Begin("Orthographic Camera")) {
        ImGui::Text("%s", &name[0]);
        ImGui::DragFloat("X Mag", &xMag);
        ImGui::DragFloat("Y Mag", &yMag);
        ImGui::End();
    }
}

/*
 * Perspective Camera
 */

PerspectiveCamera::PerspectiveCamera(std::string name) : Camera(name) {}

void PerspectiveCamera::update(float width, float height) {
    // todo should look into using calculated aspect ratio as option instead of the defined one
    if (zFar < .1f) {
        projection = glm::infinitePerspective(fov, aspectRatio, zNear);
    } else {
        projection = glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    if (ImGui::Begin("Perspective Camera")) {
        ImGui::Text("%s", &name[0]);
        ImGui::SliderFloat("FOV", &fov, 30.f, 120.f);
        ImGui::SliderFloat("Aspect Ratio", &aspectRatio, 0.f, 10.f);
        ImGui::DragFloat("Z Near", &zNear, 1.f, .1f, 1000.f);
        ImGui::SameLine(10.f);
        ImGui::DragFloat("Z Far", &zFar, 1.f, .1f, 1000.f);
        ImGui::End();
    }
}