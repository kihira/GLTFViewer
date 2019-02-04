#ifndef GLTFVIEWER_ORTHOGRAPHICCAMERA_H
#define GLTFVIEWER_ORTHOGRAPHICCAMERA_H

#include "camera.h"

class OrthographicCamera : public Camera {
public:
    float xMag;
    float yMag;

    explicit OrthographicCamera(std::string name);

private:
    void update(float width, float height) override;
};

#include "camera.h"

#endif //GLTFVIEWER_ORTHOGRAPHICCAMERA_H
