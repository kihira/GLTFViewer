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
    if (mesh != nullptr) {
        mesh->render();
    }
}
