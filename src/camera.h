#ifndef GLTFVIEWER_CAMERA_H
#define GLTFVIEWER_CAMERA_H


#include <glm/mat4x4.hpp>

class Camera {
public:
    const char *name;
    glm::mat4 projection;
    float zFar;
    float zNear;

    virtual void update(float width, float height) = 0;

protected:
    explicit Camera(const char *name);

private:
    virtual void gui() = 0;
};

class OrthographicCamera : public Camera {
public:
    float xMag;
    float yMag;

    explicit OrthographicCamera(const char *name);

private:
    void update(float width, float height) override;
};

class PerspectiveCamera : public Camera {
public:
    float aspectRatio;
    float fov;

    explicit PerspectiveCamera(const char *name);

private:
    void update(float width, float height) override;
};

#endif //GLTFVIEWER_CAMERA_H
