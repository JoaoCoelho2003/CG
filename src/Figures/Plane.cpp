#include "../../include/Plane.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

Plane::Plane(float l, int d) : length(l), divisions(d) {}

void Plane::generateVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();

    float halfLength = length / 2;
    float divisionLength = length / divisions;

    // Generate vertices for division
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {


            glm::vec3 v1, v2, v3, v4;
            v1.x = halfLength - j * divisionLength;
            v1.y = 0.0f;
            v1.z = halfLength - i * divisionLength;
            
            v2.x = halfLength - j * divisionLength;
            v2.y = 0.0f;
            v2.z = halfLength - divisionLength - i * divisionLength;
            
            v3.x = halfLength - divisionLength - j * divisionLength;
            v3.y = 0.0f;
            v3.z = halfLength - i * divisionLength;

            v4.x = halfLength - divisionLength - j * divisionLength;
            v4.y = 0.0f;
            v4.z = halfLength - divisionLength - i * divisionLength;

            // Triangle 1
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            // Triangle 2
            vertices.push_back(v2);
            vertices.push_back(v4);
            vertices.push_back(v3);      
        }
    }
}

void Plane::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    for (size_t i = 0; i < vertices.size(); ++i) {
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " " << std::endl;
    }

    outputFile.close();
}