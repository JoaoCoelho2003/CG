#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <string>

struct Vertex {
    float x, y, z;
};

class Sphere {
private:
    float radius;
    int slices;
    int stacks;
    std::vector<Vertex> vertices;

public:
    Sphere(float r, int sl, int st);

    void generateVertices();

    void writeToFile(const std::string& filename);
};

#endif
