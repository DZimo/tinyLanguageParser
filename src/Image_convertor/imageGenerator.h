#ifndef TINYLANGUAGEPARSER_IMAGEGENERATOR_H
#define TINYLANGUAGEPARSER_IMAGEGENERATOR_H

#include "../headersGeneralizer.h"
#include "../AST_checker/astNode.h"

class imageGenerator {
public:
    struct Color {
        uint8_t r, g, b;
    };

    // Now the function accepts a vector of unique pointers to astNode
    std::vector<std::vector<Color>> astToBMP(const std::vector<std::unique_ptr<astNode>>& ast);
    void saveBMP(const std::string& filename, const std::vector<std::vector<Color>>& data);
};

#endif //TINYLANGUAGEPARSER_IMAGEGENERATOR_H