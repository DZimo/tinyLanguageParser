#include "programNode.h"
#include "../AST_checker/astNode.cpp"
#include <vector>

class ProgramNode : public astNode {
public:
    std::vector<std::unique_ptr<astNode>> statements;
    std::vector<std::unique_ptr<astNode>> functions;

    explicit ProgramNode(std::vector<std::unique_ptr<astNode>> stmts)
            : statements(std::move(stmts)) {}

protected:
    std::string getType() const override { return "ProgramNode"; }

    void appendToJSON(std::ostringstream& os) const override {
        os << "\"statements\": [";
        for (size_t i = 0; i < statements.size(); ++i) {
            os << statements[i]->toJSON();
            if (i < statements.size() - 1) os << ", ";
        }
        os << "], ";

        os << "\"functions\": [";
        for (size_t i = 0; i < functions.size(); ++i) {
            os << functions[i]->toJSON();
            if (i < functions.size() - 1) os << ", ";
        }
        os << "]";
    }

    std::string getDescription() const override {
        return "Program";
    }

    void printChildren(std::ostream& os, int depth) const override {
        for (const auto& statement : statements) {
            statement->print(os, depth + 2);
        }
        for (const auto& function : functions) {
            function->print(os, depth + 2);
        }
    }

public:
    void addFunction(std::unique_ptr<astNode> func) {
        functions.push_back(std::move(func));
    }
};