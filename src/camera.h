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

class OrthographicCamera : public Camera {
public:
    float xMag;
    float yMag;

    explicit OrthographicCamera(std::string name);

private:
    void update(float width, float height) override;
};

class PerspectiveCamera : public Camera {
public:
    float aspectRatio;
    float fov;

    explicit PerspectiveCamera(std::string name);

private:
    void update(float width, float height) override;
};

#endif //GLTFVIEWER_CAMERA_H
