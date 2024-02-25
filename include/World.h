#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>

struct Model {
    std::string filename;
};

struct World {
    struct Camera {
        float posX,posY,posZ;
    } camera;

    struct Window {
        int width;
        int height;
    } window;

    std::vector<Model> models;
};

#endif