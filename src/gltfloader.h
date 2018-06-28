#include <string>
#include "node.h"

#ifndef GLTFVIEWER_GLFTLOADER_H
#define GLTFVIEWER_GLFTLOADER_H


namespace gltfloader {
    void Load(std::string filePath);

    Node *LoadNode(int id);

    Camera *LoadCamera(int id);
};

#endif