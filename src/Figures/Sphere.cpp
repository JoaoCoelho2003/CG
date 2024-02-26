#include "../../include/Sphere.h"
#include <cmath>
#include <fstream>
#include <iostream>

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
            Vertex v1, v2, v3, v4;
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
            v1.nx = sin(theta1) * cos(phi1);
            v1.ny = cos(theta1);
            v1.nz = sin(theta1) * sin(phi1);

            v2.nx = sin(theta2) * cos(phi1);
            v2.ny = cos(theta2);
            v2.nz = sin(theta2) * sin(phi1);

            v3.nx = sin(theta1) * cos(phi2);
            v3.ny = cos(theta1);
            v3.nz = sin(theta1) * sin(phi2);

            v4.nx = sin(theta2) * cos(phi2);
            v4.ny = cos(theta2);
            v4.nz = sin(theta2) * sin(phi2);

            // Texture coordinates
            v1.s = static_cast<float>(j) / slices;
            v1.t = static_cast<float>(i) / stacks;

            v2.s = static_cast<float>(j) / slices;
            v2.t = static_cast<float>(i + 1) / stacks;

            v3.s = static_cast<float>(j + 1) / slices;
            v3.t = static_cast<float>(i) / stacks;

            v4.s = static_cast<float>(j + 1) / slices;
            v4.t = static_cast<float>(i + 1) / stacks;

            // Triangle 1
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v4);

            normals.push_back(v1.nx);
            normals.push_back(v1.ny);
            normals.push_back(v1.nz);
            normals.push_back(v2.nx);
            normals.push_back(v2.ny);
            normals.push_back(v2.nz);
            normals.push_back(v4.nx);
            normals.push_back(v4.ny);
            normals.push_back(v4.nz);

            texCoords.push_back(v1.s);
            texCoords.push_back(v1.t);
            texCoords.push_back(v2.s);
            texCoords.push_back(v2.t);
            texCoords.push_back(v4.s);
            texCoords.push_back(v4.t);

            // Triangle 2
            vertices.push_back(v1);
            vertices.push_back(v3);
            vertices.push_back(v4);

            normals.push_back(v1.nx);
            normals.push_back(v1.ny);
            normals.push_back(v1.nz);
            normals.push_back(v3.nx);
            normals.push_back(v3.ny);
            normals.push_back(v3.nz);
            normals.push_back(v4.nx);
            normals.push_back(v4.ny);
            normals.push_back(v4.nz);

            texCoords.push_back(v1.s);
            texCoords.push_back(v1.t);
            texCoords.push_back(v3.s);
            texCoords.push_back(v3.t);
            texCoords.push_back(v4.s);
            texCoords.push_back(v4.t);
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
