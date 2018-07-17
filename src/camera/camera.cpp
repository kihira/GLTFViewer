#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "camera.h"
#include "../vector.hpp"

Camera::Camera(std::string name) : name(name) {
    zNear = .1f;
    zFar = 100.f;
}
