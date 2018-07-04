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
        unsigned char*data = nullptr; // Pointer to the start of the data
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
            glBindBuffer(vbo, target);
            glErrorCheck();
        }
    };

    struct Accessor {
        BufferView bufferView;
        int byteOffset = 0;
        GLenum componentType;
        int count;
        std::string type;
    };

    Asset *LoadAsset(std::string &filePath);

    Node *LoadNode(int id);

    Camera *LoadCamera(int id);

    Mesh *LoadMesh(int id);

    Accessor LoadAccessor(int id);

    BufferView LoadBufferView(int id);

    void BindPointer(Accessor accessor, GLuint index, GLuint size);
};

#endif