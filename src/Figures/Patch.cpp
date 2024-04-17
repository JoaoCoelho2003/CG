#include "../../include/Patch.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>


Patch::Patch(const std::string& filename, int tessLevel) : filename(filename), tessellationLevel(tessLevel) {}

int binomialCoefficient(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    }
    int result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

float bernstein(int n, int i, float t) {
    return binomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
}

void Patch::generateVertices() {
    vertices.clear();

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    std::vector<glm::vec3> controlPoints;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        float x, y, z;
        if (!(iss >> x >> y >> z)) {
            break;
        }
        controlPoints.push_back(glm::vec3(x, y, z));
    }

    inputFile.close();

    float step = 1.0f / tessellationLevel;
    for (float u = 0.0f; u <= 1.0f; u += step) {
        for (float v = 0.0f; v <= 1.0f; v += step) {
            glm::vec3 point(0.0f, 0.0f, 0.0f);
            for (size_t i = 0; i < controlPoints.size(); ++i) {
                for (size_t j = 0; j < controlPoints.size(); ++j) {
                    float bernsteinU = bernstein(controlPoints.size() - 1, i, u);
                    float bernsteinV = bernstein(controlPoints.size() - 1, j, v);
                    point += controlPoints[i] * bernsteinU * bernsteinV;
                }
            }
            vertices.push_back(point);
        }
    }
}

void Patch::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    for (size_t i = 0; i < vertices.size(); ++i) {
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " " << std::endl;
    }

    outputFile.close();
}