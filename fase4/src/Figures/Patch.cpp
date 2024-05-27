#include "../../include/Patch.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

Patch::Patch(const std::string& filename, int tessLevel): filename(filename), tessellationLevel(tessLevel) {}

glm::vec3 evalBezierCurve(const std::vector<glm::vec3>& P, const float &t) {
    float b0 = (1 - t) * (1 - t) * (1 - t);
    float b1 = 3 * t * (1 - t) * (1 - t);
    float b2 = 3 * t * t * (1 - t);
    float b3 = t * t * t;
    return P[0] * b0 + P[1] * b1 + P[2] * b2 + P[3] * b3;
}

glm::vec3 evalBezierCurveDerivative(const std::vector<glm::vec3>& P, const float &t) {
    float db0 = -3 * (1 - t) * (1 - t);
    float db1 = 3 * (1 - t) * (1 - t) - 6 * t * (1 - t);
    float db2 = 6 * t * (1 - t) - 3 * t * t;
    float db3 = 3 * t * t;
    return P[0] * db0 + P[1] * db1 + P[2] * db2 + P[3] * db3;
}

glm::vec3 evalBezierPatch(const std::vector<glm::vec3>& controlPoints, const float &u, const float &v) {
    std::vector<glm::vec3> uCurve(4);
    for (int i = 0; i < 4; i++) {
        uCurve[i] = evalBezierCurve({controlPoints[4*i], controlPoints[4*i+1], controlPoints[4*i+2], controlPoints[4*i+3]}, u);
    }
    return evalBezierCurve(uCurve, v);
}

glm::vec3 evalBezierPatchNormal(const std::vector<glm::vec3>& controlPoints, const float &u, const float &v) {
    std::vector<glm::vec3> uCurve(4), vCurve(4);
    std::vector<glm::vec3> uCurveDerivative(4), vCurveDerivative(4);

    for (int i = 0; i < 4; ++i) {
        uCurve[i] = evalBezierCurve({controlPoints[4*i], controlPoints[4*i+1], controlPoints[4*i+2], controlPoints[4*i+3]}, u);
        vCurve[i] = evalBezierCurve({controlPoints[i], controlPoints[4+i], controlPoints[8+i], controlPoints[12+i]}, v);

        uCurveDerivative[i] = evalBezierCurveDerivative({controlPoints[4*i], controlPoints[4*i+1], controlPoints[4*i+2], controlPoints[4*i+3]}, u);
        vCurveDerivative[i] = evalBezierCurveDerivative({controlPoints[i], controlPoints[4+i], controlPoints[8+i], controlPoints[12+i]}, v);
    }

    glm::vec3 dPdu = evalBezierCurve(uCurveDerivative, v);
    glm::vec3 dPdv = evalBezierCurve(vCurveDerivative, u);

    // Add debugging information
    if (glm::length(dPdu) == 0.0f || glm::length(dPdv) == 0.0f) {
        std::cerr << "Zero length derivative detected: dPdu = (" 
                  << dPdu.x << ", " << dPdu.y << ", " << dPdu.z << "), dPdv = ("
                  << dPdv.x << ", " << dPdv.y << ", " << dPdv.z << ")\n";
    }

    glm::vec3 normal = glm::cross(dPdu, dPdv);

    if (glm::length(normal) == 0.0f) {
        std::cerr << "Zero length normal detected: dPdu = ("
                  << dPdu.x << ", " << dPdu.y << ", " << dPdu.z << "), dPdv = ("
                  << dPdv.x << ", " << dPdv.y << ", " << dPdv.z << ")\n";
        return glm::vec3(0.0f, 0.0f, 1.0f);
    } else {
        return glm::normalize(normal);
    }
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
        std::string line;
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
        std::vector<glm::vec3> controlPointsPatch(16);
        for (int i = 0; i < 16; ++i) {
            controlPointsPatch[i] = controlPoints[patch[i]];
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

                // Check for NaN normals and handle
                if (glm::any(glm::isnan(normal0))) {
                    std::cerr << "NaN normal detected at (u0, v0): (" << u0 << ", " << v0 << ")\n";
                    normal0 = glm::vec3(0.0f, 0.0f, 1.0f);
                }
                if (glm::any(glm::isnan(normal1))) {
                    std::cerr << "NaN normal detected at (u1, v0): (" << u1 << ", " << v0 << ")\n";
                    normal1 = glm::vec3(0.0f, 0.0f, 1.0f);
                }
                if (glm::any(glm::isnan(normal2))) {
                    std::cerr << "NaN normal detected at (u1, v1): (" << u1 << ", " << v1 << ")\n";
                    normal2 = glm::vec3(0.0f, 0.0f, 1.0f);
                }
                if (glm::any(glm::isnan(normal3))) {
                    std::cerr << "NaN normal detected at (u0, v1): (" << u0 << ", " << v1 << ")\n";
                    normal3 = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                vertices.push_back(vertex0);
                vertices.push_back(vertex1);
                vertices.push_back(vertex2);

                normals.push_back(normal0);
                normals.push_back(normal1);
                normals.push_back(normal2);

                texCoords.push_back(glm::vec2(u0, v0));
                texCoords.push_back(glm::vec2(u1, v0));
                texCoords.push_back(glm::vec2(u1, v1));

                vertices.push_back(vertex0);
                vertices.push_back(vertex2);
                vertices.push_back(vertex3);

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