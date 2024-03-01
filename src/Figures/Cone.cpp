#include "../../include/Cone.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

Cone::Cone(float h, float r, int sl, int st) : height(h), radius(r), slices(sl), stacks(st) {}

void Cone::generateVertices() {
    vertices.clear();

    float height_stack = height / stacks;

    // Generate vertices for each stack and slice
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

            // Triangle 1
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            // Triangle 2
            vertices.push_back(v1);
            vertices.push_back(v3);
            vertices.push_back(v4);
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
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }

    outputFile.close();
}