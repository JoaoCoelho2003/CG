#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#ifndef VERTEX_H
#define VERTEX_H

struct Vertex {
    float x, y, z;
};

#endif

class Sphere {
private:
    float radius;
    int slices;
    int stacks;
    std::vector<Vertex> vertices;

public:
    Sphere(float r, int sl, int st) : radius(r), slices(sl), stacks(st) {}

    void generateVertices() {
        for (int i = 0; i <= stacks; ++i) {
            float theta = static_cast<float>(i) / stacks * M_PI;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int j = 0; j <= slices; ++j) {
                float phi = static_cast<float>(j) / slices * 2 * M_PI;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                Vertex vertex;
                vertex.x = radius * cosPhi * sinTheta;
                vertex.y = radius * cosTheta;
                vertex.z = radius * sinPhi * sinTheta;

                vertices.push_back(vertex);
            }
        }
    }

    void writeToFile(const std::string& filename) {
        std::ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            std::cerr << "Error opening output file." << std::endl;
            return;
        }

        for (const auto& vertex : vertices) {
            outputFile << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        }

        outputFile.close();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: generator sphere <radius> <slices> <stacks> <output_file>" << std::endl;
        return 1;
    }

    float radius = std::stof(argv[2]);
    int slices = std::stoi(argv[3]);
    int stacks = std::stoi(argv[4]);
    std::string outputFilename = argv[5];

    Sphere sphere(radius, slices, stacks);
    sphere.generateVertices();
    sphere.writeToFile(outputFilename);

    return 0;
}
