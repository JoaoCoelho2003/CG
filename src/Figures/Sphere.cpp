#include "../../include/Sphere.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

Sphere::Sphere(float r, int sl, int st) : radius(r), slices(sl), stacks(st) {}

void Sphere::generateVertices() {
    vertices.clear();
    // Generate vertices to form a sphere
    for (int i = 0; i <= slices; ++i) {
        float theta1 = i * 2 * M_PI / slices;
        float theta2 = (i + 1) * 2 * M_PI / slices;
        for (int j = 0; j <= stacks; ++j) {
            float phi1 = j * M_PI / stacks;
            float phi2 = (j + 1) * M_PI / stacks;

            // Vertices of the current quad
            glm::vec3 v1 = glm::vec3(
                radius * std::sin(phi1) * std::cos(theta1),
                radius * std::sin(phi1) * std::sin(theta1),
                radius * std::cos(phi1)
            );
            glm::vec3 v2 = glm::vec3(
                radius * std::sin(phi1) * std::cos(theta2),
                radius * std::sin(phi1) * std::sin(theta2),
                radius * std::cos(phi1)
            );
            glm::vec3 v3 = glm::vec3(
                radius * std::sin(phi2) * std::cos(theta1),
                radius * std::sin(phi2) * std::sin(theta1),
                radius * std::cos(phi2)
            );
            glm::vec3 v4 = glm::vec3(
                radius * std::sin(phi2) * std::cos(theta2),
                radius * std::sin(phi2) * std::sin(theta2),
                radius * std::cos(phi2)
            );

            // Add triangles formed by the quad
            vertices.push_back(v1);
            vertices.push_back(v3);
            vertices.push_back(v2);

            vertices.push_back(v2);
            vertices.push_back(v3);
            vertices.push_back(v4);
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
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }

    outputFile.close();
}