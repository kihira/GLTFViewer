#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "node.h"
#include "glhelper.hpp"

Node::~Node() {
    delete camera;
    delete mesh;
    for (auto &child: children) {
        delete child;
    }
    children.clear();
}

void Node::render(glm::mat4 &transform) {
    if (!isStatic) {
        matrix = glm::translate(glm::mat4(1.f), pos);
        matrix *= glm::mat4_cast(rot);
        matrix = glm::scale(matrix, scale);
    }
    transform *= matrix;

    for (auto &child : children) {
        child->render(transform);
    }

    if (mesh != nullptr) {
        glUniformMatrix4fv(glGetUniformLocation(3, "model"), 1, GL_FALSE, glm::value_ptr(transform)); // todo don't use hardcoded program ID
        mesh->render();
    }
}
