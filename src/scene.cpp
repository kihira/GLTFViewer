#include "scene.h"

void Scene::render() {
    for (auto& node: nodes) {
        node->render();
    }
}

Scene::~Scene() {
    for (auto &node: nodes) {
        delete node;
    }
    nodes.clear();
}

Scene::Scene(const std::string &name) : name(name) {}
