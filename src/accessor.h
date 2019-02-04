#ifndef GLTFVIEWER_ACCESSOR_H
#define GLTFVIEWER_ACCESSOR_H

#include <string>

class BufferView;

struct Accessor {
    BufferView *bufferView;
    int byteOffset = 0;
    unsigned int componentType;
    int count;
    std::string type;
    bool normalised = false;
};

#endif //GLTFVIEWER_ACCESSOR_H
