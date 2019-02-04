#include "asset.h"
#include "scene.h"

Asset::Asset(const std::string &name) : name(name) {}

void Asset::render() {
    if (scenes.empty()) return;

    scenes[scene]->render();
}

