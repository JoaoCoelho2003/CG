#include "../../include/Patch.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

Patch::Patch(const std::string& filename, int tessLevel)
    : filename(filename), tessellationLevel(tessLevel) {}

float bernstein(int n, int i, float u) {
    float coeff = 1.0f;
    for (int j = 1; j <= i; ++j) {
        coeff *= (float)(n - j + 1) / j;
    }
    return coeff * pow(u, i) * pow(1 - u, n - i);
}

glm::vec3 evalBezierCurve(const std::vector<glm::vec3>& P, const float &t) {
    float b0 = (1 - t) * (1 - t) * (1 - t);
    float b1 = 3 * t * (1 - t) * (1 - t);
    float b2 = 3 * t * t * (1 - t);
    float b3 = t * t * t;
    return P[0] * b0 + P[1] * b1 + P[2] * b2 + P[3] * b3;
}

glm::vec3 evalBezierPatch(const std::vector<glm::vec3>& controlPoints, const float &u, const float &v) {
    std::vector<glm::vec3> uCurve(4);
    for (int i = 0; i < 4; i++) {
        uCurve[i] = evalBezierCurve(std::vector<glm::vec3>{controlPoints.begin() + 4 * i, controlPoints.begin() + 4 * (i + 1)}, u);
    }
    return evalBezierCurve(uCurve, v);
}

void Patch::parseFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file >> numPatches;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Number of Patches: " << numPatches << std::endl;
    patches.resize(numPatches);
    for (int i = 0; i < numPatches; ++i) {
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        int index;
        std::cout << "Patch " << i << ": ";
        while (iss >> index) {
            patches[i].push_back(index);
            std::cout << index << " ";
            if (iss.peek() == ',')
                iss.ignore();
        }
        std::cout << std::endl;
    }

    int numControlPoints;
    file >> numControlPoints;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Number of Control Points: " << numControlPoints << std::endl;
    
    controlPoints.resize(numControlPoints);

    for (int i = 0; i < numControlPoints; ++i) {
        std ::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::cout << "Control Point " << i << ": ";
        iss >> controlPoints[i].x;
        iss.ignore();
        iss >> controlPoints[i].y;
        iss.ignore();
        iss >> controlPoints[i].z;
        std::cout << controlPoints[i].x << " " << controlPoints[i].y << " " << controlPoints[i].z << std::endl;
    }

    file.close();
}

void Patch::generateVertices() {
    parseFile();
    vertices.clear();

    for (const auto& patch : patches) {
        std::vector<glm::vec3> controlPointsPatch;
        for (int i = 0; i < 16; ++i) {
            int controlPointIndex = patch[i];
            controlPointsPatch.push_back(controlPoints[controlPointIndex]);
        }

        for (int j = 0; j < tessellationLevel; ++j) {
            float v0 = static_cast<float>(j) / tessellationLevel;
            float v1 = static_cast<float>(j + 1) / tessellationLevel;
            for (int i = 0; i < tessellationLevel; ++i) {
                float u0 = static_cast<float>(i) / tessellationLevel;
                float u1 = static_cast<float>(i + 1) / tessellationLevel;

                glm::vec3 vertex0 = evalBezierPatch(controlPointsPatch, u0, v0);
                glm::vec3 vertex1 = evalBezierPatch(controlPointsPatch, u1, v0);
                glm::vec3 vertex2 = evalBezierPatch(controlPointsPatch, u1, v1);
                glm::vec3 vertex3 = evalBezierPatch(controlPointsPatch, u0, v1);

                vertices.push_back(vertex0);
                vertices.push_back(vertex1);
                vertices.push_back(vertex2);

                vertices.push_back(vertex0);
                vertices.push_back(vertex2);
                vertices.push_back(vertex3);
            }
        }
    }
}


void Patch::writeToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& vertex : vertices) {
        outFile << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    outFile.close();
}

