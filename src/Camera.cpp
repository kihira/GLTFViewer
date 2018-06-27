#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "Camera.h"
#include "vector.hpp"

void Camera::update(float width, float height) {
    if (ImGui::Begin("Camera")) {
        ImGui::DragFloat3("Position", (float *)glm::value_ptr(this->pos));
        ImGui::DragFloat3("Look At", (float *)glm::value_ptr(this->lookPos));
        ImGui::SliderFloat("FOV", &this->fov, 30.f, 120.f);
        ImGui::Checkbox("Orthographic", &this->ortho);
        ImGui::End();
    }

    transform = glm::lookAt(pos, lookPos, vector::UP);

    if (this->ortho) {
        projection = glm::perspective(fov, width / height, .1f, 500.f);
    } else {
        projection = glm::ortho(0.f, width, 0.f, height);
    }
}
