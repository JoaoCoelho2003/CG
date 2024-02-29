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

    // Front face
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2 + j * step, dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2 + j * step, dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2 + (j + 1) * step, dimension / 2));

            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2 + j * step, dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2 + (j + 1) * step, dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2 + (j + 1) * step, dimension / 2));
        }
    }

    // Back face
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2 + j * step, -dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2 + (j + 1) * step, -dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2 + j * step, -dimension / 2));

            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2 + j * step, -dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2 + (j + 1) * step, -dimension / 2));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2 + (j + 1) * step, -dimension / 2));
        }
    }

    // Top face
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, dimension / 2, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, dimension / 2, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, dimension / 2, -dimension / 2 + (j + 1) * step));

            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, dimension / 2, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, dimension / 2, -dimension / 2 + (j + 1) * step));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, dimension / 2, -dimension / 2 + (j + 1) * step));
        }
    }

    // Bottom face
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2, -dimension / 2 + (j + 1) * step));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2, -dimension / 2 + j * step));

            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(-dimension / 2 + i * step, -dimension / 2, -dimension / 2 + (j + 1) * step));
            vertices.push_back(glm::vec3(-dimension / 2 + (i + 1) * step, -dimension / 2, -dimension / 2 + (j + 1) * step));
        }
    }

    // Left face
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            vertices.push_back(glm::vec3(-dimension / 2, -dimension / 2 + i * step, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(-dimension / 2, -dimension / 2 + i * step, -dimension / 2 + (j + 1) * step));
            vertices.push_back(glm::vec3(-dimension / 2, -dimension / 2 + (i + 1) * step, -dimension / 2 + j * step));

            vertices.push_back(glm::vec3(-dimension / 2, -dimension / 2 + (i + 1) * step, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(-dimension / 2, -dimension / 2 + i * step, -dimension / 2 + (j + 1) * step));
            vertices.push_back(glm::vec3(-dimension / 2, -dimension / 2 + (i + 1) * step, -dimension / 2 + (j + 1) * step));
        }
    }

    // Right face
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            vertices.push_back(glm::vec3(dimension / 2, -dimension / 2 + i * step, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(dimension / 2, -dimension / 2 + (i + 1) * step, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(dimension / 2, -dimension / 2 + i * step, -dimension / 2 + (j + 1) * step));

            vertices.push_back(glm::vec3(dimension / 2, -dimension / 2 + (i + 1) * step, -dimension / 2 + j * step));
            vertices.push_back(glm::vec3(dimension / 2, -dimension / 2 + (i + 1) * step, -dimension / 2 + (j + 1) * step));
            vertices.push_back(glm::vec3(dimension / 2, -dimension / 2 + i * step, -dimension / 2 + (j + 1) * step));
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