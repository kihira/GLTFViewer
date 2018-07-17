#ifndef GLTFVIEWER_ASSET_H
#define GLTFVIEWER_ASSET_H


#include <vector>
#include "scene.h"

/// Asset - A class that represents a glTF asset
class Asset {
public:
    const std::string name;
    short scene = 0;
    std::vector<Scene *> scenes;

    explicit Asset(const std::string &name);

    void render();
};


#endif //GLTFVIEWER_ASSET_H
