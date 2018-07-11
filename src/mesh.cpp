#include "mesh.h"
#include "glhelper.hpp"

void Primitive::render() {
    glBindVertexArray(vao);
    if (indiciesComponentType != 0) {
        glDrawElements(mode, vertices, indiciesComponentType, nullptr);
    } else {
        glDrawArrays(mode, 0, vertices);
    }
    glErrorCheck();
}

Primitive::Primitive(GLenum mode) : mode(mode) {}

Primitive::~Primitive() {
    glDeleteBuffers(1, &vao);
    // todo delete buffer views
}

Mesh::~Mesh() {
    for (auto &primitive : primitives) {
        delete primitive;
    }
    primitives.clear();
}

void Mesh::render() {
    for (auto& primitive : primitives) {
        primitive->render();
    }
}
