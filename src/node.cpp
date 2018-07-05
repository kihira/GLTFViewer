#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

Node::~Node() {
    delete camera;
    delete mesh;
    for (auto& child: children) {
        delete child;
    }
    children.clear();
}

void Node::render() {
    if (!isStatic) {
        matrix = glm::translate(glm::mat4(1.f), pos);
        matrix *= glm::mat4_cast(rot);
        matrix = glm::scale(matrix, scale);
    }

    if (mesh != nullptr) {
        mesh->render();
    }
}
