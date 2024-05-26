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

    float height_stack = height / stacks;

    for (int i = 0; i < stacks; ++i) {
        float stack_height = i * height_stack;
        float radius_at_stack = radius * (1 - stack_height / height);
        float stack_height_next = (i + 1) * height_stack;
        float radius_at_stack_next = radius * (1 - stack_height_next / height);

        for (int j = 0; j < slices; ++j) {
            float theta = j * 2 * M_PI / slices;
            float theta_next = (j + 1) * 2 * M_PI / slices;

            glm::vec3 v1, v2, v3, v4;
            v1.x = radius_at_stack * sin(theta);
            v1.y = stack_height;
            v1.z = radius_at_stack * cos(theta);

            v2.x = radius_at_stack * sin(theta_next);
            v2.y = stack_height;
            v2.z = radius_at_stack * cos(theta_next);

            v3.x = radius_at_stack_next * sin(theta_next);
            v3.y = stack_height_next;
            v3.z = radius_at_stack_next * cos(theta_next);

            v4.x = radius_at_stack_next * sin(theta);
            v4.y = stack_height_next;
            v4.z = radius_at_stack_next * cos(theta);

            // Bottom face
            if (i == 0) {
                glm::vec3 normal_bottom(0.0f, -1.0f, 0.0f);
                glm::vec3 center_bottom(0.0f, 0.0f, 0.0f);

                vertices.push_back(v1);
                vertices.push_back(center_bottom);
                vertices.push_back(v2);

                normals.push_back(normal_bottom);
                normals.push_back(normal_bottom);
                normals.push_back(normal_bottom);

                texCoords.push_back(glm::vec2(v1.x / radius + 0.5f, v1.z / radius + 0.5f));
                texCoords.push_back(glm::vec2(0.5f, 0.5f));
                texCoords.push_back(glm::vec2(v2.x / radius + 0.5f, v2.z / radius + 0.5f));
            }

            // Triangle 1
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            // Triangle 2
            vertices.push_back(v1);
            vertices.push_back(v3);
            vertices.push_back(v4);

            // Normals for each vertex
            glm::vec3 normal1 = glm::normalize(glm::vec3(v1.x, radius / height, v1.z));
            glm::vec3 normal2 = glm::normalize(glm::vec3(v2.x, radius / height, v2.z));
            glm::vec3 normal3 = glm::normalize(glm::vec3(v3.x, radius / height, v3.z));
            glm::vec3 normal4 = glm::normalize(glm::vec3(v4.x, radius / height, v4.z));

            normals.push_back(normal1);
            normals.push_back(normal2);
            normals.push_back(normal3);

            normals.push_back(normal1);
            normals.push_back(normal3);
            normals.push_back(normal4);

            // Texture coordinates
            float s1 = (float)j / slices;
            float s2 = (float)(j + 1) / slices;
            float t1 = (float)i / stacks;
            float t2 = (float)(i + 1) / stacks;

            texCoords.push_back(glm::vec2(s1, t1));
            texCoords.push_back(glm::vec2(s2, t1));
            texCoords.push_back(glm::vec2(s1, t2));

            texCoords.push_back(glm::vec2(s2, t1));
            texCoords.push_back(glm::vec2(s2, t2));
            texCoords.push_back(glm::vec2(s1, t2));
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
                   << normals[i].x << " " << normals[i].y << " " << normals[i].z << " "
                   << texCoords[i].x << " " << texCoords[i].y << std::endl;
    }

    outputFile.close();
}
