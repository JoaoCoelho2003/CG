#include "../include/all_headers.h"
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
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
        std::string completePath = "./models/" + outputFilename;
        sphere.writeToFile(completePath);
    }
    else if (figure == "cone") {
        float radius = std::stof(argv[2]);
        float height = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        int stacks = std::stoi(argv[5]);
        std::string outputFilename = argv[6];
        Cone cone(height, radius, slices, stacks);
        cone.generateVertices();
        std::string completePath = "./models/" + outputFilename;
        cone.writeToFile(completePath);
    }    
    else if (figure == "plane") {
        float length = std::stof(argv[2]);
        float divisions = std::stof(argv[3]);
        std::string outputFilename = argv[4];
        Plane plane(length, divisions);
        plane.generateVertices();
        std::string completePath = "./models/" + outputFilename;
        plane.writeToFile(completePath);
    }
    else if (figure == "box") {
        float dimension = std::stof(argv[2]);
        float divisions = std::stof(argv[3]);
        std::string outputFilename = argv[4];
        Box box(dimension, divisions);
        box.generateVertices();
        std::string completePath = "./models/" + outputFilename;
        box.writeToFile(completePath);
    }
    else if (figure == "torus") {
        float radius = std::stof(argv[2]);
        float tube_radius = std::stof(argv[3]);
        int slices = std::stof(argv[4]);
        int stacks = std::stof(argv[5]);
        std::string outputFilename = argv[6];
        Torus torus(radius, tube_radius, slices, stacks);
        torus.generateVertices();
        std::string completePath = "./models/" + outputFilename;
        torus.writeToFile(completePath);
    }
    else if (figure == "patch") {
        std::string filename = argv[2];
        double tessLevel = std::stod(argv[3]);
        std::string outputFilename = argv[4];
        std::string completeFilename = "./models/" + filename;
        Patch patch(completeFilename, tessLevel);
        patch.generateVertices();
        std::string completePath = "./models/" + outputFilename;
        patch.writeToFile(completePath);
        }
    else {
        std::cerr << "Invalid figure: " << figure << std::endl;
        return 1;
    }
    return 0;
}
