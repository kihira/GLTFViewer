#include "scene.h"

Scene::Scene(const char *name) : name(name) {}

Scene::~Scene() {
    for (auto& node: nodes) {
        delete node;
    }
    nodes.clear();
}
