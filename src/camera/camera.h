#ifndef GLTFVIEWER_CAMERA_H
#define GLTFVIEWER_CAMERA_H


#include <glm/mat4x4.hpp>
#include <string>

class Camera {
public:
    std::string name;
    glm::mat4 projection;
    float zFar;
    float zNear;

    virtual void update(float width, float height) = 0;

protected:
    explicit Camera(std::string name);
};

#endif //GLTFVIEWER_CAMERA_H
