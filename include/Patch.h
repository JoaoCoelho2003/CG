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

        std::vector<glm::vec3> vertices;

    public:
        Patch(const std::string& filename, int tessLevel);

        void generateVertices();

        void writeToFile(const std::string& filename);
};

#endif