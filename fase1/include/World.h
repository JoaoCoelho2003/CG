#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>

struct Vertex {
    float x, y, z;
};
struct Model {
    std::string filename;
    std::vector<std::vector<Vertex>> vertices;
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
    std::vector<Model> models;
};

#endif