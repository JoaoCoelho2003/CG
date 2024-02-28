#include "../../include/Sphere.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(float r, int sl, int st) : radius(r), slices(sl), stacks(st) {}

void Sphere::generateVertices() {
    // Clear any existing vertices, normals, and texture coordinates
    vertices.clear();
    normals.clear();
    texCoords.clear();

    // Generate vertices for the sphere
    for (int i = 0; i < stacks; ++i) {
        float theta1 = static_cast<float>(i) / stacks * M_PI;
        float theta2 = static_cast<float>(i + 1) / stacks * M_PI;

        for (int j = 0; j < slices; ++j) {
            float phi1 = static_cast<float>(j) / slices * 2 * M_PI;
            float phi2 = static_cast<float>(j + 1) / slices * 2 * M_PI;

            // Vertices
            glm::vec3 v1, v2, v3, v4;
            v1.x = radius * sin(theta1) * cos(phi1);
            v1.y = radius * cos(theta1);
            v1.z = radius * sin(theta1) * sin(phi1);

            v2.x = radius * sin(theta2) * cos(phi1);
            v2.y = radius * cos(theta2);
            v2.z = radius * sin(theta2) * sin(phi1);

            v3.x = radius * sin(theta1) * cos(phi2);
            v3.y = radius * cos(theta1);
            v3.z = radius * sin(theta1) * sin(phi2);

            v4.x = radius * sin(theta2) * cos(phi2);
            v4.y = radius * cos(theta2);
            v4.z = radius * sin(theta2) * sin(phi2);

            // Normals
            glm::vec3 n1 = glm::normalize(v1);
            glm::vec3 n2 = glm::normalize(v2);
            glm::vec3 n3 = glm::normalize(v3);
            glm::vec3 n4 = glm::normalize(v4);

            // Texture coordinates
            glm::vec2 t1(static_cast<float>(j) / slices, static_cast<float>(i) / stacks);
            glm::vec2 t2(static_cast<float>(j) / slices, static_cast<float>(i + 1) / stacks);
            glm::vec2 t3(static_cast<float>(j + 1) / slices, static_cast<float>(i) / stacks);
            glm::vec2 t4(static_cast<float>(j + 1) / slices, static_cast<float>(i + 1) / stacks);

            // Triangle 1
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v4);

            normals.push_back(n1.x);
            normals.push_back(n1.y);
            normals.push_back(n1.z);
            normals.push_back(n2.x);
            normals.push_back(n2.y);
            normals.push_back(n2.z);
            normals.push_back(n4.x);
            normals.push_back(n4.y);
            normals.push_back(n4.z);

            texCoords.push_back(t1.x);
            texCoords.push_back(t1.y);
            texCoords.push_back(t2.x);
            texCoords.push_back(t2.y);
            texCoords.push_back(t4.x);
            texCoords.push_back(t4.y);

            // Triangle 2
            vertices.push_back(v1);
            vertices.push_back(v3);
            vertices.push_back(v4);

            normals.push_back(n1.x);
            normals.push_back(n1.y);
            normals.push_back(n1.z);
            normals.push_back(n3.x);
            normals.push_back(n3.y);
            normals.push_back(n3.z);
            normals.push_back(n4.x);
            normals.push_back(n4.y);
            normals.push_back(n4.z);

            texCoords.push_back(t1.x);
            texCoords.push_back(t1.y);
            texCoords.push_back(t3.x);
            texCoords.push_back(t3.y);
            texCoords.push_back(t4.x);
            texCoords.push_back(t4.y);
        }
    }
}

void Sphere::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    // Write vertices, normals, and texture coordinates to the file
    for (size_t i = 0; i < vertices.size(); ++i) {
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " "
                   << normals[i * 3] << " " << normals[i * 3 + 1] << " " << normals[i * 3 + 2] << " "
                   << texCoords[i * 2] << " " << texCoords[i * 2 + 1] << std::endl;
    }

    outputFile.close();
}
