#include "../../include/Cone.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

Cone::Cone(float h, float r, int sl, int st) : height(h), radius(r), slices(sl), stacks(st) {}

void Cone::generateVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();

    // Add the apex of the cone
    vertices.push_back(glm::vec3(0.0f, 0.0f, height));
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    texCoords.push_back(0.5f);
    texCoords.push_back(0.5f);

    // Generate vertices for each stack and slice
    for (int i = 0; i <= stacks; ++i) {
        float stack_height = static_cast<float>(i) / stacks * height;
        float radius_at_stack = (1.0f - stack_height / height) * radius;

        for (int j = 0; j < slices; ++j) {
            float theta = static_cast<float>(j) / slices * 2 * M_PI;

            float x = radius_at_stack * cos(theta);
            float y = radius_at_stack * sin(theta);

            glm::vec3 vertex(x, y, stack_height);
            vertices.push_back(vertex);

            // Normals for each vertex
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, radius) - glm::vec3(0.0f, 0.0f, height));
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);

            // Texture coordinates
            float s = static_cast<float>(j) / slices;
            float t = static_cast<float>(i) / stacks;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }
}

void Cone::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    for (size_t i = 0; i < vertices.size(); ++i) {
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " "
                   << normals[i * 3] << " " << normals[i * 3 + 1] << " " << normals[i * 3 + 2] << " "
                   << texCoords[i * 2] << " " << texCoords[i * 2 + 1] << std::endl;
    }

    outputFile.close();
}