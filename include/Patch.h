#ifndef PATCH_H
#define PATCH_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Patch {
    private:
        std::string filename;
        int tessellationLevel;
        int numPatches;
        std::vector<std::vector<int>> patches;
        std::vector<glm::vec3> controlPoints;
        std::vector<glm::vec3> vertices;

    public:
        Patch(const std::string& filename, int tessLevel);

        void parseFile();

        void generateVertices();

        void writeToFile(const std::string& filename);
};

#endif