#ifndef TINYLANGUAGEPARSER_IMAGEGENERATOR_H
#define TINYLANGUAGEPARSER_IMAGEGENERATOR_H

#include "../headersGeneralizer.h"
#include "../AST_checker/astNode.h"

class imageGenerator {
public:
    struct Color {
        uint8_t r, g, b;
    };

    std::vector<std::vector<Color>> astToBMP(const std::vector<std::unique_ptr<astNode>>& ast);
    void drawAST(const std::unique_ptr<astNode>& node, int x, int y, int level, std::vector<std::vector<imageGenerator::Color>>& data);
    void saveBMP(const std::string& filename, const std::vector<std::vector<Color>>& data);
};

#endif //TINYLANGUAGEPARSER_IMAGEGENERATOR_H