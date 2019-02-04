#ifndef GLTFVIEWER_ASSET_H
#define GLTFVIEWER_ASSET_H


#include <vector>
#include <string>

class Scene;
class Node;
class Mesh;

/**
 * Asset - A class that represents a glTF asset
 *
 * vectors are used for storing scenes, nodes and meshes instead of maps to try and reduce memory impact as we expect
 * that the exporter for the glTF file would have stripped unused nodes.
 * TODO should use maps? If that's the case, should just use a cache on the loader
 */
class Asset {
public:
    const std::string name;
    short scene = 0;
    std::vector<Scene *> scenes;
    std::vector<Node *> nodes;
    std::vector<Mesh *> meshes;

    explicit Asset(const std::string &name);

    void render();
};


#endif //GLTFVIEWER_ASSET_H
