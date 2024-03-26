#ifndef Tree_H
#define Tree_H

#include <vector>

enum class TransformationType {
    TRANSLATE,
    ROTATE,
    SCALE
};

struct Transformation {
    TransformationType type;
    std::vector<float> values;
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
