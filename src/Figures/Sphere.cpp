#include "../../include/Sphere.h"
#include <cmath>
#include <fstream>
#include <iostream>

Sphere::Sphere(float r, int sl, int st) : radius(r), slices(sl), stacks(st) {}

void Sphere::generateVertices() {
    // Clear any existing vertices
    vertices.clear();

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

            vertices.push_back(vertex);
        }
    }
}

void Sphere::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    for (const auto& vertex : vertices) {
        outputFile << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    outputFile.close();
}
