#include "../../include/Patch.h"
#include <fstream>
#include <iostream>
#include <sstream>

Patch::Patch(const std::string& filename, int tessLevel)
    : filename(filename), tessellationLevel(tessLevel) {}

float bernstein(int n, int i, float u) {
    float coeff = 1.0f;
    for (int j = 1; j <= i; ++j) {
        coeff *= (float)(n - j + 1) / j;
    }
    return coeff * pow(u, i) * pow(1 - u, n - i);
}

void Patch::generateVertices() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file >> numPatches;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Number of Patches: " << numPatches << std::endl;
    std::vector<std::vector<int>> patches(numPatches);
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
    
    std::vector<glm::vec3> controlPoints(numControlPoints);

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

    vertices.clear();

    int n = 3;
    float step = 1.0f / tessellationLevel;

    for (int p = 0; p < numPatches; ++p) {
        for (float u = 0.0f; u <= 1.0f; u += step) {
            for (float v = 0.0f; v <= 1.0f; v += step) {
                glm::vec3 point(0.0f, 0.0f, 0.0f);
                for (int i = 0; i <= n; ++i) {
                    for (int j = 0; j <= n; ++j) {
                        float bernsteinU = bernstein(n, i, u);
                        float bernsteinV = bernstein(n, j, v);
                        int idx = patches[p][i * (n + 1) + j];
                        if (idx < controlPoints.size()) {
                            point += controlPoints[idx] * bernsteinU * bernsteinV;
                        }
                    }
                }
                vertices.push_back(point);
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

    for (int p = 0; p < numPatches; ++p) {
        for (int i = 0; i < tessellationLevel; ++i) {
            for (int j = 0; j < tessellationLevel; ++j) {
                int index = i * tessellationLevel + j;
                outFile << vertices[p * tessellationLevel * tessellationLevel + index].x << " "
                        << vertices[p * tessellationLevel * tessellationLevel + index].y << " "
                        << vertices[p * tessellationLevel * tessellationLevel + index].z << std::endl;
            }
        }
    }

    outFile.close();
}
