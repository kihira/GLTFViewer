#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "gltfloader.h"

using json = nlohmann::json;

void gltfloader::Load(std::string filePath) {
    if (!filePath.compare(filePath.length() - 4, 3, "glb")) {
        std::cerr << "Can only load GLB files currently" << std::endl;
        return;
    }

    // Open file and get length
    std::ifstream file;
    file.open(filePath, std::ios::in|std::ios::binary|std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    std::streampos fileLength = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file header
    uint32_t magic, version, length = 0;
    file.read(reinterpret_cast<char*>(&magic), sizeof(uint32_t));
    file.read(reinterpret_cast<char*>(&version), sizeof(uint32_t));
    file.read(reinterpret_cast<char*>(&length), sizeof(uint32_t));

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

    // Read chunk header
    uint32_t chunkLength, chunkType = 0;
    file.read(reinterpret_cast<char*>(&chunkLength), sizeof(uint32_t));
    file.read(reinterpret_cast<char*>(&chunkType), sizeof(uint32_t));

    if (chunkType != 0x4E4F534A) {
        std::cerr << "Expected JSON chunk but didn't get it" << std::endl;
        return;
    }

    std::cout << "Reading JSON chunk of length " << chunkLength << std::endl;

    // Read chunk data
    char* chunkData = new char[chunkLength];
    file.read(chunkData, chunkLength - 1);
    file.seekg(1, std::ios::cur); // Skip one byte which I believe is the string terminator. Can't have it part of the data array as causes malformed string

    json j;
    try {
        j = json::parse(chunkData);
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON data: " << e.what() << std::endl;
        return;
    }

    // Read binary chunk (if exists)
    if (file.tellg() != length) {
        file.read(reinterpret_cast<char*>(&chunkLength), sizeof(uint32_t));
        file.read(reinterpret_cast<char*>(&chunkType), sizeof(uint32_t));

        if (chunkType != 0x004E4942) {
            std::cerr << "Expected binary chunk but didn't get it" << std::endl;
            return;
        }

        std::cout << "Reading binary chunk of length " << chunkLength << std::endl;
    }

    file.close();

    std::cout << j << std::endl;
}
