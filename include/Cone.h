#ifndef CONE_H
#define CONE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Cone {
private:
    float height;
    float radius;
    int slices;
    int stacks;

    std::vector<glm::vec3> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

public:
    Cone(float h, float r, int sl, int st);

    void generateVertices();

    void writeToFile(const std::string& filename);
};

#endif
