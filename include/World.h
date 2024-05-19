#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>
#include <map>
#include "Tree.h"
#include <GL/glew.h>
#include <IL/il.h>

struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float s, t;
};

struct Triangle {
    Vertex v1, v2, v3;
};

struct Material {
    float diffuse[3] = {200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f};
    float ambient[3] = {50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f};
    float specular[3] = {0.0f, 0.0f, 0.0f};
    float emissive[3] = {0.0f, 0.0f, 0.0f};
    float shininess = 0.0f;
};

struct Model {
    std::string filename;
    std::vector<Triangle> triangles;
    GLuint vbo;
    GLuint textureID;
    Material material;
};

struct Light {
    float position[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
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
    std::vector<Light> lights;
};

#endif