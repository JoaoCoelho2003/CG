#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>
#include <map>
#include "Tree.h"
#include <GL/glew.h>

struct Vertex {
    float x, y, z;
};

struct Triangle {
    Vertex v1, v2, v3;
};

struct Model {
    std::string filename;
    std::vector<Triangle> triangles;
    GLuint vbo;
};

struct Camera {
    float posX, posY, posZ;
    float lookAtX, lookAtY, lookAtZ;
    float upX, upY, upZ;
    float fov, near, far;
};

struct Window {
    int width;
    int height;
};

struct World {
    Camera camera;
    Window window;
    std::map<std::string, Model> models;
    Tree tree;
};

#endif