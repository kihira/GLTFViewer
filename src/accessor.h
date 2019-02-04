#ifndef GLTFVIEWER_ACCESSOR_H
#define GLTFVIEWER_ACCESSOR_H

#include "bufferview.h"

struct Accessor {
    BufferView *bufferView;
    int byteOffset = 0;
    unsigned int componentType;
    int count;
    std::basic_string<char> type;
    bool normalised = false;
};

#endif //GLTFVIEWER_ACCESSOR_H
