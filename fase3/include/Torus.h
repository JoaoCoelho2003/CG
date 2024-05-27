#ifndef TORUS_H
#define TORUS_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Torus {
private:
    float radius;
    float tube_radius;
    int slices;
    int stacks;

    std::vector<glm::vec3> vertices;

public:
    Torus(float r,float tr, int sl, int st);

    void generateVertices();

    void writeToFile(const std::string& filename);
};

#endif