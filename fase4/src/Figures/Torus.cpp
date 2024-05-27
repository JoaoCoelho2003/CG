#include "../../include/Torus.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

Torus::Torus(float r,float tr, int s, int sl): radius(r), tube_radius(tr), slices(s), stacks(sl) {}

void Torus::generateVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();

    float phi_step = 2 * M_PI / float(slices);
    float theta_step = 2 * M_PI / float(stacks);

    for (int i = 0; i <= slices; ++i) {
        float curr_phi = i * phi_step;
        float next_phi = (i + 1) * phi_step;

        for (int j = 0; j <= stacks; ++j) {
            float curr_theta = j * theta_step;
            float next_theta = (j + 1) * theta_step;

            // Generate vertices
            glm::vec3 v1 = glm::vec3(
                (radius + tube_radius * cos(curr_theta)) * cos(curr_phi),
                tube_radius * sin(curr_theta),
                (radius + tube_radius * cos(curr_theta)) * sin(curr_phi)
            );

            glm::vec3 v2 = glm::vec3(
                (radius + tube_radius * cos(next_theta)) * cos(curr_phi),
                tube_radius * sin(next_theta),
                (radius + tube_radius * cos(next_theta)) * sin(curr_phi)
            );

            glm::vec3 v3 = glm::vec3(
                (radius + tube_radius * cos(next_theta)) * cos(next_phi),
                tube_radius * sin(next_theta),
                (radius + tube_radius * cos(next_theta)) * sin(next_phi)
            );

            glm::vec3 v4 = glm::vec3(
                (radius + tube_radius * cos(curr_theta)) * cos(next_phi),
                tube_radius * sin(curr_theta),
                (radius + tube_radius * cos(curr_theta)) * sin(next_phi)
            );

            // Calculate normals
            glm::vec3 n1 = glm::normalize(v1 - glm::vec3(radius * cos(curr_phi), 0, radius * sin(curr_phi)));
            glm::vec3 n2 = glm::normalize(v2 - glm::vec3(radius * cos(curr_phi), 0, radius * sin(curr_phi)));
            glm::vec3 n3 = glm::normalize(v3 - glm::vec3(radius * cos(next_phi), 0, radius * sin(next_phi)));
            glm::vec3 n4 = glm::normalize(v4 - glm::vec3(radius * cos(next_phi), 0, radius * sin(next_phi)));

            // Calculate texture coordinates
            glm::vec2 t1 = glm::vec2((float)i / slices, (float)j / stacks);
            glm::vec2 t2 = glm::vec2((float)i / slices, (float)(j + 1) / stacks);
            glm::vec2 t3 = glm::vec2((float)(i + 1) / slices, (float)(j + 1) / stacks);
            glm::vec2 t4 = glm::vec2((float)(i + 1) / slices, (float)j / stacks);

            // Add triangles formed by the quad
            vertices.push_back(v1);
            normals.push_back(n1);
            texCoords.push_back(t1);

            vertices.push_back(v2);
            normals.push_back(n2);
            texCoords.push_back(t2);

            vertices.push_back(v4);
            normals.push_back(n4);
            texCoords.push_back(t4);

            vertices.push_back(v2);
            vertices.push_back(v3);
            vertices.push_back(v4);

            normals.push_back(n2);
            normals.push_back(n3);
            normals.push_back(n4);

            texCoords.push_back(t2);
            texCoords.push_back(t3);
            texCoords.push_back(t4);
        }
    }
}

void Torus::writeToFile(const std::string& filename) {
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