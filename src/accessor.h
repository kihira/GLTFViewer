#ifndef GLTFVIEWER_ACCESSOR_H
#define GLTFVIEWER_ACCESSOR_H

struct Accessor {
    BufferView *bufferView;
    int byteOffset = 0;
    unsigned int componentType;
    int count;
    basic_string<char> type;
    bool normalised = false;
};

#include "bufferview.h"

#endif //GLTFVIEWER_ACCESSOR_H
