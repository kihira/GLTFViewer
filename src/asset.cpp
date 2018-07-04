#include "asset.h"

void Asset::render() {
    if (scenes.empty()) return;

    scenes[scene]->render();
}

Asset::Asset(const std::string &name) : name(name) {}
