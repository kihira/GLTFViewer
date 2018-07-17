#ifndef GLTFVIEWER_NODE_H
#define GLTFVIEWER_NODE_H


#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "camera/camera.h"
#include "mesh.h"

class Node {
public:
    virtual ~Node();

    void render(glm::mat4 &transform);

public:
    const char *name;
    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::quat rot = glm::quat(0, 0, 0, 1);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::mat4 matrix = glm::mat4(1.f);
    std::vector<Node *> children;
    Camera *camera;
    Mesh *mesh;
    bool isStatic; // Whether the matrix will ever change
};


#endif //GLTFVIEWER_NODE_H
