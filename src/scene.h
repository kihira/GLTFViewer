#ifndef GLTFVIEWER_SCENE_H
#define GLTFVIEWER_SCENE_H


#include <string>
#include "node.h"

class Scene {
public:
    const std::string name;
    std::vector<Node *> nodes;

    Scene(const std::string &name);

    virtual ~Scene();

    void render();
};


#endif //GLTFVIEWER_SCENE_H
