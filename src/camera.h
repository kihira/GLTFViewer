#ifndef GLTFVIEWER_CAMERA_H
#define GLTFVIEWER_CAMERA_H


#include <glm/mat4x4.hpp>

class Camera {
public:
    const char *name;
    glm::mat4 transform;
    glm::mat4 projection;

    void update(float width, float height);

    explicit Camera(const char *name);

private:
    glm::vec3 pos = glm::vec3();
    glm::vec3 lookPos = glm::vec3();
    float fov = 70.f;
    bool ortho = false;
};

#endif //GLTFVIEWER_CAMERA_H
