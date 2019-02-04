#include "camera.h"

Camera::Camera(std::string name) : name(name) {
    zNear = .1f;
    zFar = 100.f;
}
