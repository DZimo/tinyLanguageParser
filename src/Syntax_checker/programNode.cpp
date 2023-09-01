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

    void appendToJSON(std::ostringstream& os, int indent = 0) const override {
        std::string indentStr(indent, ' ');

        os << indentStr << "\"statements\": [\n";
        for (size_t i = 0; i < statements.size(); ++i) {
            os << statements[i]->toJSON(indent + 2);
            if (i < statements.size() - 1) os << ",\n";
        }
        os << "\n" << indentStr << "],\n";

        os << indentStr << "\"functions\": [\n";
        for (size_t i = 0; i < functions.size(); ++i) {
            os << functions[i]->toJSON(indent + 2);
            if (i < functions.size() - 1) os << ",\n";
        }
        os << "\n" << indentStr << "]";
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