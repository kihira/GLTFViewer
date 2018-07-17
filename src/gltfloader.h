#include <string>
#include "node.h"
#include "mesh.h"
#include "asset.h"
#include "bufferview.h"
#include "accessor.h"

#ifndef GLTFVIEWER_GLFTLOADER_H
#define GLTFVIEWER_GLFTLOADER_H


namespace gltf {

    Asset *LoadAsset(std::string &filePath);

    /**
     * Loads data for a Node from the asset if already loaded, if not, loads it from json data and saves it to the asset
     * @param asset The asset
     * @param id The node id
     * @return The node data
     */
    Node *LoadNode(Asset &asset, int id);

    Camera *LoadCamera(Asset &asset, int id);

    Mesh *LoadMesh(Asset &asset, int id);

    Accessor LoadAccessor(Asset &asset, int id);

    BufferView *LoadBufferView(Asset &asset, int id);

    void BindPointer(Accessor &accessor, GLuint index, GLuint size);
};

#endif