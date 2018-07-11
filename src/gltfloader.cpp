#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "gltfloader.h"
#include "camera.h"
#include "node.h"
#include "glhelper.hpp"
#include "asset.h"

nlohmann::json jsonData;
std::vector<unsigned char> binData;
std::map<int, gltf::BufferView *> bufferViewCache;

Asset *gltf::LoadAsset(std::string &filePath) {
    if (!filePath.compare(filePath.length() - 4, 3, "glb")) {
        std::cerr << "Can only load GLB files currently" << std::endl;
        return nullptr;
    }

    // Open file and get length
    std::ifstream file;
    file.open(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return nullptr;
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
        return nullptr;
    }
    if (magic != 0x46546C67) {
        std::cerr << "Invalid file type, expected GLB file but got something else" << std::endl;
        return nullptr;
    }
    if (version != 2) {
        std::cerr << "Only Binary glTF version 2 is currently supported" << std::endl;
        return nullptr;
    }
    std::cout << "Loading GLB file of size " << length << std::endl;

    // Read JSON chunk header
    uint32_t chunkLength, chunkType = 0;
    file.read(reinterpret_cast<char *>(&chunkLength), sizeof(uint32_t));
    file.read(reinterpret_cast<char *>(&chunkType), sizeof(uint32_t));

    if (chunkType != 0x4E4F534A) {
        std::cerr << "Expected JSON chunk but didn't get it" << std::endl;
        return nullptr;
    }

    std::cout << "Reading JSON chunk of length " << chunkLength << std::endl;

    // Read JSON chunk data
    std::vector<unsigned char> data(chunkLength);
    file.read(reinterpret_cast<char *>(&data[0]), chunkLength);

    try {
        jsonData = nlohmann::json::parse(data);
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse JSON data: " << e.what() << std::endl;
        return nullptr;
    }

    // Read binary chunk (if exists)
    if (file.tellg() != length) {
        file.read(reinterpret_cast<char *>(&chunkLength), sizeof(uint32_t));
        file.read(reinterpret_cast<char *>(&chunkType), sizeof(uint32_t));

        if (chunkType != 0x004E4942) {
            std::cerr << "Expected binary chunk but didn't get it" << std::endl;
            return nullptr;
        }

        std::cout << "Reading binary chunk of length " << chunkLength << std::endl;
        binData = std::vector<unsigned char>(chunkLength);
        file.read(reinterpret_cast<char *>(&binData[0]), chunkLength);
    }

    file.close();

    // Validate asset
    if (jsonData["asset"] == nullptr || jsonData["asset"]["version"] != "2.0") {
        std::cerr << "Only glTF version 2 files are supported" << std::endl;
        return nullptr;
    }
    if (jsonData.find("extensionsRequired") != jsonData.end()) {
        std::cerr << "Unable to load asset as it requires extensions not available" << std::endl;
    }

    auto *asset = new Asset(filePath); // todo get just filename

    // Load scenes
    if (jsonData.find("scenes") != jsonData.end()) {
        for (auto &elem: jsonData["scenes"]) {
            Scene *scene = new Scene(elem.value("name", "Scene"));
            asset->scenes.push_back(scene);
            if (elem.find("nodes") == elem.end()) continue;
            for (int nodeId : elem["nodes"]) {
                scene->nodes.push_back(LoadNode(nodeId));
            }
        }
    }
    asset->scene = jsonData.value("scene", 0);

    // Purge cache todo always clear if there is an error
    bufferViewCache.clear();

    return asset;
}

Node *gltf::LoadNode(int id) {
    nlohmann::json nodeData = jsonData["nodes"][id];
    auto *node = new Node();

    if (nodeData.find("children") != nodeData.end()) {
        for (auto &i : nodeData["children"]) {
            node->children.push_back(LoadNode(i));
        }
    }

    if (nodeData.find("matrix") != nodeData.end()) {
        std::vector<float> matrix = nodeData["matrix"];
        node->matrix = glm::make_mat4(&matrix[0]);
        node->isStatic = true;
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

    if (nodeData.find("mesh") != nodeData.end()) {
        node->mesh = LoadMesh(nodeData["mesh"]);
    }

    return node;
}


Camera *gltf::LoadCamera(int id) {
    nlohmann::json cameraData = jsonData["cameras"][id];

    if (cameraData["type"] == "perspective") {
        std::string name = cameraData.find("name") != cameraData.end() ? cameraData["name"] : "Perspective Camera";
        PerspectiveCamera *camera = new PerspectiveCamera(name);
        camera->aspectRatio = cameraData["aspectRatio"];
        camera->fov = cameraData["yfov"];
        camera->zNear = cameraData["znear"];
        if (cameraData.find("zfar") != cameraData.end()) {
            camera->zFar = cameraData["zfar"];
        }

        return camera;
    } else {
        std::string name = cameraData.find("name") != cameraData.end() ? cameraData["name"] : "Ortho Camera";
        OrthographicCamera *camera = new OrthographicCamera(name);
        camera->xMag = cameraData["xmag"];
        camera->yMag = cameraData["ymag"];
        camera->zNear = cameraData["znear"];
        camera->zFar = cameraData["zfar"];

        return camera;
    }
}

Mesh *gltf::LoadMesh(int id) {
    nlohmann::json meshData = jsonData["meshes"][id];
    auto *mesh = new Mesh();

    for (auto &primitiveData : meshData["primitives"]) {
        nlohmann::json attributes = primitiveData["attributes"];
        auto *primitive = new Primitive((GLenum) primitiveData.value("mode", GL_TRIANGLES));

        glGenVertexArrays(1, &primitive->vao);
        glBindVertexArray(primitive->vao);
        glErrorCheck();

        // Load attributes data
        if (attributes.find("POSITION") != attributes.end()) {
            Accessor accessor = LoadAccessor(attributes["POSITION"]);
            primitive->vertices = accessor.count;
            BindPointer(accessor, 0, 3);
        }

        if (attributes.find("NORMAL") != attributes.end()) {
            // BindPointer(LoadAccessor(attributes["NORMAL"]), 1, 3);
        } else {
            // todo generate normals
        }

        if (attributes.find("TANGENT") != attributes.end()) {
            BindPointer(LoadAccessor(attributes["TANGENT"]), 2, 3);
        } else {
            // todo generate tangents
        }

        if (attributes.find("TEXCOORD_0") != attributes.end()) {
            BindPointer(LoadAccessor(attributes["TEXCOORD_0"]), 3, 2);
        }

        if (attributes.find("TEXCOORD_1") != attributes.end()) {
            BindPointer(LoadAccessor(attributes["TEXCOORD_1"]), 4, 2);
        }

        // Load indices data (if exists)
        if (primitiveData.find("indices") != primitiveData.end()) {
            Accessor accessor = LoadAccessor(primitiveData["indices"]);
            accessor.bufferView->target = GL_ELEMENT_ARRAY_BUFFER;
            accessor.bufferView->bind();
            primitive->indiciesComponentType = accessor.componentType;
        }

        mesh->primitives.push_back(primitive);
    }

    return mesh;
}

void gltf::BindPointer(Accessor accessor, GLuint index, GLuint size) {
    accessor.bufferView->bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, accessor.componentType, GL_FALSE, accessor.bufferView->byteStride,
                          reinterpret_cast<const void *>(accessor.byteOffset));
    glErrorCheck();
}

gltf::Accessor gltf::LoadAccessor(int id) {
    nlohmann::json accessorData = jsonData["accessors"][id];

    gltf::Accessor accessor;
    accessor.bufferView = LoadBufferView(accessorData["bufferView"]);
    accessor.byteOffset = accessorData.value("byteOffset", 0);
    accessor.componentType = accessorData["componentType"];
    accessor.count = accessorData["count"];
    accessor.type = accessorData["type"];

    return accessor;
}

gltf::BufferView *gltf::LoadBufferView(int id) {
    if (bufferViewCache.find(id) != bufferViewCache.end()) {
        return bufferViewCache[id];
    }

    nlohmann::json bfData = jsonData["bufferViews"][id];

    auto *bufferView = new BufferView();
    bufferView->byteLength = bfData["byteLength"];
    bufferView->byteStride = bfData.value("byteStride", 0);
    bufferView->data = &binData[bfData.value("byteOffset", 0)];
    bufferView->target = static_cast<GLuint>(bfData.value("target", GL_ARRAY_BUFFER));

    bufferViewCache[id] = bufferView;

    return bufferView;
}