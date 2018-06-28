#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gltfloader.h"
#include "camera.h"
#include "node.h"
#include "scene.h"

nlohmann::json jsonData;

void gltfloader::Load(std::string filePath) {
    if (!filePath.compare(filePath.length() - 4, 3, "glb")) {
        std::cerr << "Can only load GLB files currently" << std::endl;
        return;
    }

    // Open file and get length
    std::ifstream file;
    file.open(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    std::streampos fileLength = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file header
    uint32_t magic, version, length = 0;
    file.read(reinterpret_cast<char *>(&magic), sizeof(uint32_t));
    file.read(reinterpret_cast<char *>(&version), sizeof(uint32_t));
    file.read(reinterpret_cast<char *>(&length), sizeof(uint32_t));

    if (fileLength != length) {
        std::cerr << "Expected file of length " << length << " but got " << fileLength << std::endl;
        return;
    }
    if (magic != 0x46546C67) {
        std::cerr << "Invalid file type, expected GLB file but got something else" << std::endl;
        return;
    }
    if (version != 2) {
        std::cerr << "Only Binary glTF version 2 is currently supported" << std::endl;
        return;
    }
    std::cout << "Loading GLB file of size " << length << std::endl;

    // Read JSON chunk header
    uint32_t chunkLength, chunkType = 0;
    file.read(reinterpret_cast<char *>(&chunkLength), sizeof(uint32_t));
    file.read(reinterpret_cast<char *>(&chunkType), sizeof(uint32_t));

    if (chunkType != 0x4E4F534A) {
        std::cerr << "Expected JSON chunk but didn't get it" << std::endl;
        return;
    }

    std::cout << "Reading JSON chunk of length " << chunkLength << std::endl;

    // Read JSON chunk data
    std::vector<unsigned char> data(chunkLength);
    file.read(reinterpret_cast<char *>(&data[0]), chunkLength);

    try {
        jsonData = nlohmann::json::parse(data);
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse JSON data: " << e.what() << std::endl;
        return;
    }

    // Read binary chunk (if exists)
    if (file.tellg() != length) {
        file.read(reinterpret_cast<char *>(&chunkLength), sizeof(uint32_t));
        file.read(reinterpret_cast<char *>(&chunkType), sizeof(uint32_t));

        if (chunkType != 0x004E4942) {
            std::cerr << "Expected binary chunk but didn't get it" << std::endl;
            return;
        }

        std::cout << "Reading binary chunk of length " << chunkLength << std::endl;
        std::vector<unsigned char> binData(chunkLength);
        file.read(reinterpret_cast<char *>(&binData[0]), chunkLength);
    }

    file.close();

    // Validate asset
    if (jsonData["asset"] == nullptr || jsonData["asset"]["version"] != "2.0") {
        std::cerr << "Only glTF version 2 files are supported" << std::endl;
        return;
    }
    if (jsonData.find("extensionsRequired") != jsonData.end()) {
        std::cerr << "Unable to load asset as it requires extensions not available" << std::endl;
    }

    // Load scenes
    if (jsonData.find("scenes") != jsonData.end()) {
        for (auto &elem: jsonData["scenes"]) {
            Scene *scene = new Scene(elem.find("name") != elem.end() ? elem["name"] : "Scene");
            if (elem.find("nodes") == elem.end()) continue;
            for (int nodeId : elem["nodes"]) {
                scene->nodes.push_back(LoadNode(nodeId));
            }
        }
    }
}

Node *gltfloader::LoadNode(int id) {
    nlohmann::json nodeData = jsonData["nodes"][id];
    Node *node = new Node();

    if (nodeData.find("matrix") != nodeData.end()) {
        // todo load matrix data
    } else {
        if (nodeData.find("translation") != nodeData.end()) {
            // todo look into casting instead?
            node->pos.x = nodeData["translation"][0];
            node->pos.y = nodeData["translation"][1];
            node->pos.z = nodeData["translation"][2];
        }
        if (nodeData.find("rotation") != nodeData.end()) {
            node->rot.x = nodeData["rotation"][0];
            node->rot.y = nodeData["rotation"][1];
            node->rot.z = nodeData["rotation"][2];
            node->rot.w = nodeData["rotation"][3];
        }
        if (nodeData.find("scale") != nodeData.end()) {
            node->scale.x = nodeData["scale"][0];
            node->scale.y = nodeData["scale"][1];
            node->scale.z = nodeData["scale"][2];
        }
    }

    if (nodeData.find("camera") != nodeData.end()) {
        node->camera = LoadCamera(nodeData["camera"]);
    }

    return node;
}


Camera* gltfloader::LoadCamera(int id) {
    nlohmann::json cameraData = jsonData["cameras"][id];

    if (cameraData["type"] == "perspective") {
        PerspectiveCamera *camera = new PerspectiveCamera(cameraData["name"]);
        camera->aspectRatio = cameraData["aspectRatio"];
        camera->fov = cameraData["yfov"];
        camera->zNear = cameraData["znear"];
        if (cameraData.find("zfar") != cameraData.end()) {
            camera->zFar = cameraData["zfar"];
        }

        return camera;
    }
    else {
        OrthographicCamera *camera = new OrthographicCamera(cameraData["name"]);
        camera->xMag = cameraData["xmag"];
        camera->yMag = cameraData["ymag"];
        camera->zNear = cameraData["znear"];
        camera->zFar = cameraData["zfar"];

        return camera;
    }
}
