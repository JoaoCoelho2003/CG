#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <string>
#include "Vertex.h"

class Sphere {
private:
    float radius;
    int slices;
    int stacks;

    std::vector<Vertex> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

public:
    Sphere(float r, int sl, int st);

    void generateVertices();

    void writeToFile(const std::string& filename);
};

#endif
