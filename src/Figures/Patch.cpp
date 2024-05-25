#include "../../include/Patch.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

Patch::Patch(const std::string& filename, int tessLevel)
    : filename(filename), tessellationLevel(tessLevel) {}

glm::vec3 evalBezierCurve(const std::vector<glm::vec3>& P, const float &t) {
    float b0 = (1 - t) * (1 - t) * (1 - t);
    float b1 = 3 * t * (1 - t) * (1 - t);
    float b2 = 3 * t * t * (1 - t);
    float b3 = t * t * t;
    return P[0] * b0 + P[1] * b1 + P[2] * b2 + P[3] * b3;
}

glm::vec3 evalBezierPatchNormal(const std::vector<glm::vec3>& controlPoints, const float &u, const float &v) {
    std::vector<glm::vec3> uCurve(4), vCurve(4);
    for (int i = 0; i < 4; ++i) {
        uCurve[i] = evalBezierCurve(std::vector<glm::vec3>{controlPoints.begin() + 4 * i, controlPoints.begin() + 4 * (i + 1)}, u);
        vCurve[i] = evalBezierCurve(std::vector<glm::vec3>{controlPoints.begin() + i, controlPoints.begin() + i + 4}, v);
    }
    glm::vec3 dPdu = glm::normalize(evalBezierCurve(uCurve, v + 0.01f) - evalBezierCurve(uCurve, v - 0.01f));
    glm::vec3 dPdv = glm::normalize(evalBezierCurve(vCurve, u + 0.01f) - evalBezierCurve(vCurve, u - 0.01f));
    return glm::normalize(glm::cross(dPdu, dPdv));
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
    patches.resize(numPatches);
    for (int i = 0; i < numPatches; ++i) {
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        int index;
        while (iss >> index) {
            patches[i].push_back(index);
            if (iss.peek() == ',')
                iss.ignore();
        }
    }

    int numControlPoints;
    file >> numControlPoints;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    controlPoints.resize(numControlPoints);

    for (int i = 0; i < numControlPoints; ++i) {
        std ::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        iss >> controlPoints[i].x;
        iss.ignore();
        iss >> controlPoints[i].y;
        iss.ignore();
        iss >> controlPoints[i].z;
    }

    file.close();
}

void Patch::generateVertices() {
    parseFile();
    vertices.clear();
    normals.clear();
    texCoords.clear();

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

                glm::vec3 normal0 = evalBezierPatchNormal(controlPointsPatch, u0, v0);
                glm::vec3 normal1 = evalBezierPatchNormal(controlPointsPatch, u1, v0);
                glm::vec3 normal2 = evalBezierPatchNormal(controlPointsPatch, u1, v1);
                glm::vec3 normal3 = evalBezierPatchNormal(controlPointsPatch, u0, v1);

                vertices.push_back(vertex0);
                vertices.push_back(vertex1);
                vertices.push_back(vertex2);

                vertices.push_back(vertex0);
                vertices.push_back(vertex2);
                vertices.push_back(vertex3);

                normals.push_back(normal0);
                normals.push_back(normal1);
                normals.push_back(normal2);

                texCoords.push_back(glm::vec2(u0, v0));
                texCoords.push_back(glm::vec2(u1, v0));
                texCoords.push_back(glm::vec2(u1, v1));

                normals.push_back(normal0);
                normals.push_back(normal2);
                normals.push_back(normal3);

                texCoords.push_back(glm::vec2(u0, v0));
                texCoords.push_back(glm::vec2(u1, v1));
                texCoords.push_back(glm::vec2(u0, v1));
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

    for (size_t i = 0; i < vertices.size(); ++i) {
        outFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " "
                << normals[i].x << " " << normals[i].y << " " << normals[i].z << " "
                << texCoords[i].x << " " << texCoords[i].y << std::endl;
    }

    outFile.close();
}

