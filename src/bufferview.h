#ifndef GLTFVIEWER_BUFFERVIEW_H
#define GLTFVIEWER_BUFFERVIEW_H

#include "glhelper.hpp"

struct BufferView {
    unsigned int vbo = 0;
    unsigned char *data = nullptr; // Pointer to the start of the data
    int byteLength = 0;
    int byteStride = 0;
    unsigned int target = GL_ARRAY_BUFFER;

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

#include "glhelper.hpp"

#endif //GLTFVIEWER_BUFFERVIEW_H
