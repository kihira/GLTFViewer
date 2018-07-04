#ifndef GLTFVIEWER_MESH_H
#define GLTFVIEWER_MESH_H


#include <glad/glad.h>
#include <vector>

class Primitive {
public:
    const GLenum mode;
    GLuint vao;
    int vertices;
    bool hasIndicies;

    explicit Primitive(GLenum mode);

    virtual ~Primitive();

    void render();
};

class Mesh {
public:
    const char *name;
    std::vector<Primitive *> primitives;

    virtual ~Mesh();

    void render();
};

#endif //GLTFVIEWER_MESH_H
