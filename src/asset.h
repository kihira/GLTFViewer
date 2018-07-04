#ifndef GLTFVIEWER_ASSET_H
#define GLTFVIEWER_ASSET_H


#include <vector>
#include "scene.h"

/// Asset - A class that represents a glTF asset
class Asset {
public:
    const std::string name;
    std::vector<Scene *> scenes;
    short scene = 0;

    explicit Asset(const std::string &name);

    void render();
};


#endif //GLTFVIEWER_ASSET_H
