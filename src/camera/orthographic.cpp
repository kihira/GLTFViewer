#include "orthographic.h"

OrthographicCamera::OrthographicCamera(std::__cxx11::string name) : Camera(name) {}

void OrthographicCamera::update(float width, float height) {
    //projection = glm::ortho(0.f, xMag, 0.f, yMag);
    projection[0][0] = 1.f / xMag;
    projection[1][1] = 1.f / yMag;
    projection[2][2] = 2.f / (zNear / zFar);
    projection[3][2] = (zFar + zNear) / (zNear / zFar);

/*    if (ImGui::Begin("Orthographic Camera")) { todo
        ImGui::Text("%s", &name[0]);
        ImGui::DragFloat("X Mag", &xMag);
        ImGui::DragFloat("Y Mag", &yMag);
        ImGui::End();
    }*/
}