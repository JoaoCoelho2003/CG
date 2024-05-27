#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Sphere {
private:
    float radius;
    int slices;
    int stacks;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

public:
    Sphere(float r, int sl, int st);

    void generateVertices();

    void writeToFile(const std::string& filename);
};

#endif