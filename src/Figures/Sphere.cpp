#include "../../include/Sphere.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

Sphere::Sphere(float r, int sl, int st) : radius(r), slices(sl), stacks(st) {}

void Sphere::generateVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();

    for (int i = 0; i <= slices; ++i) {
        float theta1 = i * 2 * M_PI / slices;
        float theta2 = (i + 1) * 2 * M_PI / slices;
        for (int j = 0; j < stacks; ++j) {
            float phi1 = j * M_PI / stacks;
            float phi2 = (j + 1) * M_PI / stacks;

            glm::vec3 v1 = glm::vec3(
                radius * std::sin(phi1) * std::cos(theta1),
                radius * std::cos(phi1),
                radius * std::sin(phi1) * std::sin(theta1)
            );
            glm::vec3 v2 = glm::vec3(
                radius * std::sin(phi1) * std::cos(theta2),
                radius * std::cos(phi1),
                radius * std::sin(phi1) * std::sin(theta2)
            );
            glm::vec3 v3 = glm::vec3(
                radius * std::sin(phi2) * std::cos(theta1),
                radius * std::cos(phi2),
                radius * std::sin(phi2) * std::sin(theta1)
            );
            glm::vec3 v4 = glm::vec3(
                radius * std::sin(phi2) * std::cos(theta2),
                radius * std::cos(phi2),
                radius * std::sin(phi2) * std::sin(theta2)
            );

            // Calculate normals (normalized positions)
            glm::vec3 n1 = glm::normalize(v1);
            glm::vec3 n2 = glm::normalize(v2);
            glm::vec3 n3 = glm::normalize(v3);
            glm::vec3 n4 = glm::normalize(v4);

            // Calculate texture coordinates
            glm::vec2 t1 = glm::vec2((float)i / slices, (float)j / stacks);
            glm::vec2 t2 = glm::vec2((float)(i + 1) / slices, (float)j / stacks);
            glm::vec2 t3 = glm::vec2((float)i / slices, (float)(j + 1) / stacks);
            glm::vec2 t4 = glm::vec2((float)(i + 1) / slices, (float)(j + 1) / stacks);

            // Add triangles formed by the quad
            vertices.push_back(v1);
            normals.push_back(n1);
            texCoords.push_back(t1);

            vertices.push_back(v2);
            normals.push_back(n2);
            texCoords.push_back(t2);

            vertices.push_back(v3);
            normals.push_back(n3);
            texCoords.push_back(t3);

            vertices.push_back(v2);
            normals.push_back(n2);
            texCoords.push_back(t2);

            vertices.push_back(v4);
            normals.push_back(n4);
            texCoords.push_back(t4);

            vertices.push_back(v3);
            normals.push_back(n3);
            texCoords.push_back(t3);
        }
    }
}

void Sphere::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " "
                   << normals[i].x << " " << normals[i].y << " " << normals[i].z << " "
                   << texCoords[i].x << " " << texCoords[i].y << std::endl;
    }

    outputFile.close();
}
