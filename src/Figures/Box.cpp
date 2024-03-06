#define _USE_MATH_DEFINES
#include "math.h"
#include "../../include/Box.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

Box::Box(float d, float div) : dimension(d), divisions(div) {}

void Box::generateVertices() {
    // Calculate step size for divisions
    float step = dimension / divisions;

    // Generate vertices for each face

    float half_dim = dimension / 2.0f;
    // Front face
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float y = -half_dim + j * step;
            float y_next = -half_dim + (j + 1) * step;

            vertices.push_back(glm::vec3(x, y, half_dim));
            vertices.push_back(glm::vec3(x_next, y, half_dim));
            vertices.push_back(glm::vec3(x,y_next, half_dim));

            vertices.push_back(glm::vec3(x_next, y, half_dim));
            vertices.push_back(glm::vec3(x_next,y_next, half_dim));
            vertices.push_back(glm::vec3(x,y_next, half_dim));
        }
    }

    // Back face
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;
        for (int j = 0; j < divisions; ++j) {
            float y = -half_dim + j * step;
            float y_next = -half_dim + (j + 1) * step;

            vertices.push_back(glm::vec3(x, y, -half_dim));
            vertices.push_back(glm::vec3(x, y_next, -half_dim));
            vertices.push_back(glm::vec3(x_next, y, -half_dim));

            vertices.push_back(glm::vec3(x_next, y, -half_dim));
            vertices.push_back(glm::vec3(x, y_next, -half_dim));
            vertices.push_back(glm::vec3(x_next, y_next, -half_dim));
        }
    }

    // Top face
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;
        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;

            vertices.push_back(glm::vec3(x, half_dim, z));
            vertices.push_back(glm::vec3(x, half_dim, z_next));
            vertices.push_back(glm::vec3(x_next, half_dim, z));

            vertices.push_back(glm::vec3(x_next, half_dim, z));
            vertices.push_back(glm::vec3(x, half_dim, z_next));
            vertices.push_back(glm::vec3(x_next, half_dim, z_next));
        }
    }

    // Bottom face
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;

            vertices.push_back(glm::vec3(x, -half_dim, z_next));
            vertices.push_back(glm::vec3(x, -half_dim, z));
            vertices.push_back(glm::vec3(x_next, -half_dim, z));

            vertices.push_back(glm::vec3(x_next, -half_dim, z_next));
            vertices.push_back(glm::vec3(x, -half_dim, z_next));
            vertices.push_back(glm::vec3(x_next, -half_dim, z));
        }
    }

    // Left face
    for (int i = 0; i < divisions; ++i) {
        float y = -half_dim + i * step;
        float y_next = -half_dim + (i + 1) * step;
        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;

            vertices.push_back(glm::vec3(-half_dim, y, z));
            vertices.push_back(glm::vec3(-half_dim, y, z_next));
            vertices.push_back(glm::vec3(-half_dim, y_next, z));

            vertices.push_back(glm::vec3(-half_dim, y_next, z));
            vertices.push_back(glm::vec3(-half_dim, y, z_next));
            vertices.push_back(glm::vec3(-half_dim, y_next, z_next));
        }
    }

    // Right face
    for (int i = 0; i < divisions; ++i) {
        float y = -half_dim + i * step;
        float y_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;


            vertices.push_back(glm::vec3(half_dim, y, z));
            vertices.push_back(glm::vec3(half_dim, y_next, z));
            vertices.push_back(glm::vec3(half_dim, y, z_next));

            vertices.push_back(glm::vec3(half_dim, y_next, z));
            vertices.push_back(glm::vec3(half_dim, y_next, z_next));
            vertices.push_back(glm::vec3(half_dim, y, z_next));
        }
    }
}


void Box::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    for (size_t i = 0; i < vertices.size(); ++i) {
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }
}