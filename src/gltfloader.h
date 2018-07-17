#include <string>
#include "node.h"
#include "mesh.h"
#include "glhelper.hpp"
#include "asset.h"

#ifndef GLTFVIEWER_GLFTLOADER_H
#define GLTFVIEWER_GLFTLOADER_H


namespace gltf {
    struct BufferView {
        GLuint vbo = 0;
        unsigned char *data = nullptr; // Pointer to the start of the data
        int byteLength = 0;
        int byteStride = 0;
        GLuint target = GL_ARRAY_BUFFER;

        void bind() {
            if (vbo == 0) {
                glGenBuffers(1, &vbo);
                glBindBuffer(target, vbo);
                glBufferData(target, byteLength, data, GL_STATIC_DRAW);
                glErrorCheck();
                return;
            }
            glBindBuffer(target, vbo);
            glErrorCheck();
        }
    };

    struct Accessor {
        BufferView *bufferView;
        int byteOffset = 0;
        GLenum componentType;
        int count;
        std::string type;
        bool normalised = false;
    };

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

    void BindPointer(Accessor accessor, GLuint index, GLuint size);
};

#endif