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
    for (int i = 0; i <= stacks; ++i) {
        float theta = static_cast<float>(i) / stacks * M_PI;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= slices; ++j) {
            float phi = static_cast<float>(j) / slices * 2 * M_PI;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vertex vertex;
            vertex.x = radius * cosPhi * sinTheta;
            vertex.y = radius * cosTheta;
            vertex.z = radius * sinPhi * sinTheta;

            // Calculate normal
            vertex.nx = cosPhi * sinTheta;
            vertex.ny = cosTheta;
            vertex.nz = sinPhi * sinTheta;

            // Calculate texture coordinates
            vertex.s = static_cast<float>(j) / slices;
            vertex.t = static_cast<float>(i) / stacks;

            // Store vertex, normal, and texture coordinate
            vertices.push_back(vertex);
            normals.push_back(vertex.nx);
            normals.push_back(vertex.ny);
            normals.push_back(vertex.nz);
            texCoords.push_back(vertex.s);
            texCoords.push_back(vertex.t);
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
