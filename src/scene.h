#ifndef GLTFVIEWER_SCENE_H
#define GLTFVIEWER_SCENE_H


#include <vector>
#include "node.h"

class Scene {
public:
    const char *name;
    std::vector<Node *> nodes;

    Scene(const char *name);

    virtual ~Scene();
};


#endif //GLTFVIEWER_SCENE_H
