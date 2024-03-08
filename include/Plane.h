#ifndef Plane_H
#define Plane_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Plane {
private:
    float length;
    int divisions;

    std::vector<glm::vec3> vertices;

public:
    Plane(float h, int l);

    void generateVertices();

    void writeToFile(const std::string& filename);
};

#endif
