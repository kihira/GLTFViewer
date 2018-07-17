#ifndef GLTFVIEWER_PERSPECTIC_H
#define GLTFVIEWER_PERSPECTIC_H


#include "camera.h"

class PerspectiveCamera : public Camera {
public:
    float aspectRatio;
    float fov;

    explicit PerspectiveCamera(std::string name);

private:
    void update(float width, float height) override;
};


#endif //GLTFVIEWER_PERSPECTIC_H
