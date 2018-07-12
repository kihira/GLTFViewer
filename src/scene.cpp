#include "scene.h"

void Scene::render() {
    for (auto& node: nodes) {
        glm::mat4 transform(1.f);
        node->render(transform);
    }
}

Scene::~Scene() {
    for (auto &node: nodes) {
        delete node;
    }
    nodes.clear();
}

Scene::Scene(const std::string &name) : name(name) {}
