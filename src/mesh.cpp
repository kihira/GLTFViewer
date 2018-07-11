#include "mesh.h"
#include "glhelper.hpp"

void Primitive::render() {
    glBindVertexArray(vao);
    if (hasIndicies) {
        glDrawElements(mode, vertices, GL_UNSIGNED_INT, nullptr);// todo could be short or byte as well
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
