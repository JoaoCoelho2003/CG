#ifndef Tree_H
#define Tree_H

using namespace std;
#include <glm/glm.hpp>

#include <vector>

enum class TransformationType {
    TRANSLATE,
    ROTATE,
    SCALE,
    CATMULL_ROM_TRANSLATE,
    TIME_DEPENDENT_ROTATE
};

struct Transformation {
    TransformationType type;
    std::vector<float> values;
    bool align;
    std::vector<glm::vec3> control_points;
};
struct Node {
    std::vector<Transformation> transformations;
    std::vector<std::string> model_name;
};


struct Tree {
    Node node;
    std::vector<Tree> children;
};

#endif
