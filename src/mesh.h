#ifndef GLTFVIEWER_MESH_H
#define GLTFVIEWER_MESH_H


#include <glad/glad.h>
#include <vector>

class Primitive {
public:
    const GLenum mode;
    GLuint vao;
    int vertices;
    GLenum indiciesComponentType; // This is a non zero value if we are using an indices buffer

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
