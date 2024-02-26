#include "../../include/Cone.h"
#include <cmath>
#include <fstream>
#include <iostream>


Cone::Cone(float h, float r, int sl, int st) : height(h), radius(r), slices(sl), stacks(st) {}

void Cone::generateVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();

    for (int i = 0; i < stacks; ++i) {
        float theta1 = static_cast<float>(i) / stacks * 2 * M_PI;
        float theta2 = static_cast<float>(i + 1) / stacks * 2 * M_PI;

        for (int j = 0; j < slices; ++j) {
            float phi1 = static_cast<float>(j) / slices * radius;
            float phi2 = static_cast<float>(j + 1) / slices * radius;

            Vertex v1, v2, v3, v4;
            v1.x = 0.0f;
            v1.y = 0.0f;
            v1.z = 0.0f;

            v2.x = phi1 * cos(theta1);
            v2.y = phi1 * sin(theta1);
            v2.z = 0.0f;

            v3.x = phi2 * cos(theta1);
            v3.y = phi2 * sin(theta1);
            v3.z = 0.0f;

            v4.x = phi1 * cos(theta2);
            v4.y = phi1 * sin(theta2);
            v4.z = 0.0f;

            v1.nx = 0.0f;
            v1.ny = 0.0f;
            v1.nz = -1.0f;

            v2.nx = cos(theta1);
            v2.ny = sin(theta1);
            v2.nz = 0.0f;

            v3.nx = cos(theta1);
            v3.ny = sin(theta1);
            v3.nz = 0.0f;

            v4.nx = cos(theta2);
            v4.ny = sin(theta2);
            v4.nz = 0.0f;

            v1.s = 0.5f;
            v1.t = 0.5f;

            v2.s = (1 + cos(theta1)) / 2;
            v2.t = (1 + sin(theta1)) / 2;

            v3.s = (1 + cos(theta1)) / 2;
            v3.t = (1 + sin(theta1)) / 2;

            v4.s = (1 + cos(theta2)) / 2;
            v4.t = (1 + sin(theta2)) / 2;

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            normals.push_back(v1.nx);
            normals.push_back(v1.ny);
            normals.push_back(v1.nz);
            normals.push_back(v2.nx);
            normals.push_back(v2.ny);
            normals.push_back(v2.nz);
            normals.push_back(v3.nx);
            normals.push_back(v3.ny);
            normals.push_back(v3.nz);

            texCoords.push_back(v1.s);
            texCoords.push_back(v1.t);
            texCoords.push_back(v2.s);
            texCoords.push_back(v2.t);
            texCoords.push_back(v3.s);
            texCoords.push_back(v3.t);

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