#include <string>
#include "node.h"
#include "mesh.h"

#ifndef GLTFVIEWER_GLFTLOADER_H
#define GLTFVIEWER_GLFTLOADER_H


namespace gltfloader {
    struct BufferView {
        GLuint vbo = 0;
        int byteLength = 0;
        int byteStride = 0;
    };

    struct Accessor {
        BufferView bufferView;
        int byteOffset;
        GLenum componentType;
        int count;
        std::string type;
    };

    void Load(std::string filePath);

    Node *LoadNode(int id);

    Camera *LoadCamera(int id);

    Mesh *LoadMesh(int id);

    Accessor LoadAccessor(int id);

    BufferView LoadBufferView(int id);

    void BindPointer(Accessor accessor, GLuint index, GLuint size);
};

#endif