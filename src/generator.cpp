#include "../include/Sphere.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: generator sphere <radius> <slices> <stacks> <output_file>" << std::endl;
        return 1;
    }

    // if the figure is a sphere
    // ./generator sphere 1 10 10 sphere_1_10_10.3d

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
    else {
        std::cerr << "Invalid figure: " << figure << std::endl;
        return 1;
    }
    return 0;
}
