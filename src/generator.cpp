#include "../include/Sphere.h"
#include "../include/Cone.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

int main(int argc, char* argv[]) {
    std::string figure = argv[1];

    if (figure == "sphere") {
        float radius = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        std::string outputFilename = argv[5];
        Sphere sphere(radius, slices, stacks);
        sphere.generateVertices();
        sphere.writeToFile(outputFilename);
    }
    else if (figure == "cone") {
        float height = std::stof(argv[2]);
        float radius = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        int stacks = std::stoi(argv[5]);
        std::string outputFilename = argv[6];
        Cone cone(height, radius, slices, stacks);
        cone.generateVertices();
        cone.writeToFile(outputFilename);
    }
    else {
        std::cerr << "Invalid figure: " << figure << std::endl;
        return 1;
    }
    return 0;
}
