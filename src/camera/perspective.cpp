#include "perspective.h"

PerspectiveCamera::PerspectiveCamera(std::string name) : Camera(name) {
    aspectRatio = 16.f/9.f;
    fov = 70.f;
}

void PerspectiveCamera::update(float width, float height) {
    // todo should look into using calculated aspect ratio as option instead of the defined one
    if (zFar < .1f) {
        projection = glm::infinitePerspective(fov, aspectRatio, zNear);
    } else {
        projection = glm::perspective(fov, aspectRatio, zNear, zFar);
    }

/*    if (ImGui::Begin("Perspective Camera")) { todo
        ImGui::Text("%s", &name[0]);
        ImGui::SliderFloat("FOV", &fov, 30.f, 120.f);
        ImGui::SliderFloat("Aspect Ratio", &aspectRatio, 0.f, 10.f);
        ImGui::DragFloat("Z Near", &zNear, 1.f, .1f, 1000.f);
        ImGui::SameLine(10.f);
        ImGui::DragFloat("Z Far", &zFar, 1.f, .1f, 1000.f);
        ImGui::End();
    }*/
}
