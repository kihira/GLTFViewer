#pragma once

#include <glm/vec3.hpp>

namespace vector {
    const glm::vec3 UP(0.f, 1.f, 0.f); // NOLINT
    const glm::vec3 DOWN(0.f, -1.f, 0.f); // NOLINT
    const glm::vec3 RIGHT(1.f, 0.f, 0.f); // NOLINT
    const glm::vec3 LEFT(-1.f, 0.f, 0.f); // NOLINT
    const glm::vec3 BACKWARD(0.f, 0.f, 1.f); // NOLINT
    const glm::vec3 FORWARD(0.f, 0.f, -1.f); // NOLINT
}