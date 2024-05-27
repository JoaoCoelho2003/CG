#ifndef BOX_H
#define BOX_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Box {
private:
    float dimension;
    float divisions;

    std::vector<glm::vec3> vertices;

public:
    Box(float dim, float div);

    void generateVertices();

    void writeToFile(const std::string& filename);
};

#endif
