//
// Created by Admin on 8/13/2023.
//

#include "programNode.h"
#include "../AST_checker/astNode.cpp"
#include <vector>


class programNode : public astNode {
public:
    std::vector<std::unique_ptr<astNode>> statements;
    std::vector<std::unique_ptr<astNode>> functions;

    programNode(std::vector<std::unique_ptr<astNode>> stmts)
    : statements(std::move(stmts)) {}

    std::string toJSON() const override {
        std::string result = "[";
        for (size_t i = 0; i < statements.size(); ++i) {
            result += statements[i]->toJSON();
            if (i < statements.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }

    void print(std::ostream& os, int depth = 0) const override {
        return;
    }

    void addFunction(std::unique_ptr<astNode> func) {
        functions.push_back(std::move(func));
    }
};