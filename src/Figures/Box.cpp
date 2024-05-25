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
    vertices.clear();
    normals.clear();
    texCoords.clear();
    
    // Calculate step size for divisions
    float step = dimension / divisions;
    float half_dim = dimension / 2.0f;

    // Front face
    glm::vec3 normal_front(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float y = -half_dim + j * step;
            float y_next = -half_dim + (j + 1) * step;

            // Vertices
            vertices.push_back(glm::vec3(x, y, half_dim));
            vertices.push_back(glm::vec3(x_next, y, half_dim));
            vertices.push_back(glm::vec3(x, y_next, half_dim));

            vertices.push_back(glm::vec3(x_next, y, half_dim));
            vertices.push_back(glm::vec3(x_next, y_next, half_dim));
            vertices.push_back(glm::vec3(x, y_next, half_dim));

            // Normals (same normal for all vertices of this face)
            for (int k = 0; k < 6; ++k) {
                normals.push_back(normal_front);
            }

            // Texture coordinates
            float s = (float)i / divisions;
            float s_next = (float)(i + 1) / divisions;
            float t = (float)j / divisions;
            float t_next = (float)(j + 1) / divisions;

            texCoords.push_back(glm::vec2(s, t));
            texCoords.push_back(glm::vec2(s_next, t));
            texCoords.push_back(glm::vec2(s, t_next));

            texCoords.push_back(glm::vec2(s_next, t));
            texCoords.push_back(glm::vec2(s_next, t_next));
            texCoords.push_back(glm::vec2(s, t_next));
        }
    }

    // Back face
    glm::vec3 normal_back(0.0f, 0.0f, -1.0f);
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float y = -half_dim + j * step;
            float y_next = -half_dim + (j + 1) * step;

            // Vertices
            vertices.push_back(glm::vec3(x, y, -half_dim));
            vertices.push_back(glm::vec3(x, y_next, -half_dim));
            vertices.push_back(glm::vec3(x_next, y, -half_dim));

            vertices.push_back(glm::vec3(x_next, y, -half_dim));
            vertices.push_back(glm::vec3(x, y_next, -half_dim));
            vertices.push_back(glm::vec3(x_next, y_next, -half_dim));

            // Normals
            for (int k = 0; k < 6; ++k) {
                normals.push_back(normal_back);
            }

            // Texture coordinates
            float s = (float)i / divisions;
            float s_next = (float)(i + 1) / divisions;
            float t = (float)j / divisions;
            float t_next = (float)(j + 1) / divisions;

            texCoords.push_back(glm::vec2(s, t));
            texCoords.push_back(glm::vec2(s, t_next));
            texCoords.push_back(glm::vec2(s_next, t));

            texCoords.push_back(glm::vec2(s_next, t));
            texCoords.push_back(glm::vec2(s, t_next));
            texCoords.push_back(glm::vec2(s_next, t_next));
        }
    }

    // Top face
    glm::vec3 normal_top(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;

            // Vertices
            vertices.push_back(glm::vec3(x, half_dim, z));
            vertices.push_back(glm::vec3(x, half_dim, z_next));
            vertices.push_back(glm::vec3(x_next, half_dim, z));

            vertices.push_back(glm::vec3(x_next, half_dim, z));
            vertices.push_back(glm::vec3(x, half_dim, z_next));
            vertices.push_back(glm::vec3(x_next, half_dim, z_next));

            // Normals
            for (int k = 0; k < 6; ++k) {
                normals.push_back(normal_top);
            }

            // Texture coordinates
            float s = (float)i / divisions;
            float s_next = (float)(i + 1) / divisions;
            float t = (float)j / divisions;
            float t_next = (float)(j + 1) / divisions;

            texCoords.push_back(glm::vec2(s, t));
            texCoords.push_back(glm::vec2(s, t_next));
            texCoords.push_back(glm::vec2(s_next, t));

            texCoords.push_back(glm::vec2(s_next, t));
            texCoords.push_back(glm::vec2(s, t_next));
            texCoords.push_back(glm::vec2(s_next, t_next));
        }
    }

    // Bottom face
    glm::vec3 normal_bottom(0.0f, -1.0f, 0.0f);
    for (int i = 0; i < divisions; ++i) {
        float x = -half_dim + i * step;
        float x_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;

            // Vertices
            vertices.push_back(glm::vec3(x, -half_dim, z_next));
            vertices.push_back(glm::vec3(x, -half_dim, z));
            vertices.push_back(glm::vec3(x_next, -half_dim, z));

            vertices.push_back(glm::vec3(x_next, -half_dim, z_next));
            vertices.push_back(glm::vec3(x, -half_dim, z_next));
            vertices.push_back(glm::vec3(x_next, -half_dim, z));

            // Normals
            for (int k = 0; k < 6; ++k) {
                normals.push_back(normal_bottom);
            }

            // Texture coordinates
            float s = (float)i / divisions;
            float s_next = (float)(i + 1) / divisions;
            float t = (float)j / divisions;
            float t_next = (float)(j + 1) / divisions;

            texCoords.push_back(glm::vec2(s, t_next));
            texCoords.push_back(glm::vec2(s, t));
            texCoords.push_back(glm::vec2(s_next, t));

            texCoords.push_back(glm::vec2(s_next, t_next));
            texCoords.push_back(glm::vec2(s, t_next));
            texCoords.push_back(glm::vec2(s_next, t));
        }
    }

    // Left face
    glm::vec3 normal_left(-1.0f, 0.0f, 0.0f);
    for (int i = 0; i < divisions; ++i) {
        float y = -half_dim + i * step;
        float y_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;

            // Vertices
            vertices.push_back(glm::vec3(-half_dim, y, z));
            vertices.push_back(glm::vec3(-half_dim, y, z_next));
            vertices.push_back(glm::vec3(-half_dim, y_next, z));

            vertices.push_back(glm::vec3(-half_dim, y_next, z));
            vertices.push_back(glm::vec3(-half_dim, y, z_next));
            vertices.push_back(glm::vec3(-half_dim, y_next, z_next));

            // Normals
            for (int k = 0; k < 6; ++k) {
                normals.push_back(normal_left);
            }

            // Texture coordinates
            float s = (float)j / divisions;
            float s_next = (float)(j + 1) / divisions;
            float t = (float)i / divisions;
            float t_next = (float)(i + 1) / divisions;

            texCoords.push_back(glm::vec2(0.0f, s));
            texCoords.push_back(glm::vec2(0.0f, s_next));
            texCoords.push_back(glm::vec2(1.0f, s));

            texCoords.push_back(glm::vec2(1.0f, s));
            texCoords.push_back(glm::vec2(0.0f, s_next));
            texCoords.push_back(glm::vec2(1.0f, s_next));
        }
    }

    // Right face
    glm::vec3 normal_right(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < divisions; ++i) {
        float y = -half_dim + i * step;
        float y_next = -half_dim + (i + 1) * step;

        for (int j = 0; j < divisions; ++j) {
            float z = -half_dim + j * step;
            float z_next = -half_dim + (j + 1) * step;

            // Vertices
            vertices.push_back(glm::vec3(half_dim, y, z));
            vertices.push_back(glm::vec3(half_dim, y_next, z));
            vertices.push_back(glm::vec3(half_dim, y, z_next));

            vertices.push_back(glm::vec3(half_dim, y_next, z));
            vertices.push_back(glm::vec3(half_dim, y_next, z_next));
            vertices.push_back(glm::vec3(half_dim, y, z_next));

            // Normals
            for (int k = 0; k < 6; ++k) {
                normals.push_back(normal_right);
            }

            // Texture coordinates
            float s = (float)j / divisions;
            float s_next = (float)(j + 1) / divisions;
            float t = (float)i / divisions;
            float t_next = (float)(i + 1) / divisions;
    
            texCoords.push_back(glm::vec2(0.0f, s));
            texCoords.push_back(glm::vec2(1.0f, s_next));
            texCoords.push_back(glm::vec2(0.0f, s_next));

            texCoords.push_back(glm::vec2(0.0f, s));
            texCoords.push_back(glm::vec2(1.0f, s));
            texCoords.push_back(glm::vec2(1.0f, s_next));
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
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " "
                   << normals[i].x << " " << normals[i].y << " " << normals[i].z << " "
                   << texCoords[i].x << " " << texCoords[i].y << std::endl;
    }
}
