//
// Created by Admin on 8/13/2023.
//

#ifndef TINYLANGUAGEPARSER_ASTNODE_H
#define TINYLANGUAGEPARSER_ASTNODE_H


#include <memory>
#include <vector>
#include "../Lexical_checker/tokenizer.h"

class astNode {

public:
    virtual ~astNode() = default;  // Virtual destructor
    virtual std::string toJSON() const = 0;
    virtual void print(std::ostream& os, int depth = 0) const = 0;
};

class DeclarationNode : public astNode {
    std::string dataType;
    std::string name;

public:
    DeclarationNode(const std::string &dataType, const std::string &name)
            : dataType(dataType), name(name) {}
    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Declaration: " << dataType << " " << name << std::endl;
    }
    std::string toJSON() const override {
        return "{ \"type\": \"Declaration\", \"name\": \"" + name + "\", \"dataType\": \"" + dataType + "\" }";
    }
};

class identifierNode : public astNode {
    std::string name;

public:
    identifierNode(const std::string &name) : name(name) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Identifier: " << name << std::endl;
    }
    std::string toJSON() const override {
        return "{ \"type\": \"Identifier\", \"name\": \"" + name + "\" }";
    }
};

// ... similarly for other AST nodes...
class BinaryOpNode : public astNode {
public:
    std::unique_ptr<astNode> left;
    std::unique_ptr<astNode> right;
    TokenType op;

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "BinaryOp: " << tokenTypeToString(op) << std::endl;

        left->print(os, depth + 1);  // Recursively print the left subtree
        right->print(os, depth + 1); // Recursively print the right subtree
    }
    std::string toJSON() const override {
        return "{ \"type\": \"BinaryExpression\", \"operator\": \"" + tokenTypeToString(op) +
               "\", \"left\": " + left->toJSON() + ", \"right\": " + right->toJSON() + " }";
    }

    BinaryOpNode(std::unique_ptr<astNode> left, TokenType op, std::unique_ptr<astNode> right)
            : left(std::move(left)), op(op), right(std::move(right)) {}
};

class NumberNode : public astNode {
public:
    std::string value;

    explicit NumberNode(const std::string& value) : value(value) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Number: " << value << std::endl;
    }
    std::string toJSON() const override {
        return "{ \"type\": \"Number\", \"value\": \"" + value + "\" }";
    }

};

class VariableNode : public astNode {
public:
    std::string name;

    explicit VariableNode(const std::string& name) : name(name) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Variable: " << name << std::endl;
    }
    std::string toJSON() const override {
        return "{ \"type\": \"Variable\", \"name\": \"" + name + "\" }";
    }
};

// Block Statements
class BlockNode : public astNode {

public:
    explicit BlockNode(std::vector<std::unique_ptr<astNode>> statements)
            : statements(std::move(statements)) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Block:" << std::endl;
        for (const auto& stmt : statements) {
            stmt->print(os, depth + 1);
        }
    }

    std::string toJSON() const override {
        std::string json = "{ \"type\": \"Block\", \"statements\": [";
        for (size_t i = 0; i < statements.size(); ++i) {
            json += statements[i]->toJSON();
            if (i < statements.size() - 1) json += ", ";
        }
        json += "] }";
        return json;
    }

    std::vector<std::unique_ptr<astNode>> statements;
};

// If Statements
class IfNode : public astNode {
    std::unique_ptr<astNode> condition;
    std::unique_ptr<astNode> trueBranch;
    std::unique_ptr<astNode> falseBranch;

public:
    IfNode(std::unique_ptr<astNode> condition, std::unique_ptr<astNode> trueBranch, std::unique_ptr<astNode> falseBranch = nullptr)
            : condition(std::move(condition)), trueBranch(std::move(trueBranch)), falseBranch(std::move(falseBranch)) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "If Condition:" << std::endl;
        condition->print(os, depth + 1);
        os << "True Branch:" << std::endl;
        trueBranch->print(os, depth + 1);
        if (falseBranch) {
            os << "False Branch:" << std::endl;
            falseBranch->print(os, depth + 1);
        }
    }

    std::string toJSON() const override {
        std::string json = "{ \"type\": \"IfStatement\", \"condition\": ";
        json += condition->toJSON();
        json += ", \"trueBranch\": ";
        json += trueBranch->toJSON();
        if (falseBranch) {
            json += ", \"falseBranch\": ";
            json += falseBranch->toJSON();
        }
        json += " }";
        return json;
    }

};

// While Statements
class WhileNode : public astNode {
    std::unique_ptr<astNode> condition;
    std::unique_ptr<astNode> loopBody;

public:
    WhileNode(std::unique_ptr<astNode> condition, std::unique_ptr<astNode> loopBody)
            : condition(std::move(condition)), loopBody(std::move(loopBody)) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "While Condition:" << std::endl;
        condition->print(os, depth + 1);
        os << "Loop Body:" << std::endl;
        loopBody->print(os, depth + 1);
    }

    std::string toJSON() const override {
        std::string json = "{ \"type\": \"WhileLoop\", \"condition\": ";
        json += condition->toJSON();
        json += ", \"body\": ";
        json += loopBody->toJSON();
        json += " }";
        return json;
    }
};

// Unary Operation Nodes
class UnaryOpNode : public astNode {
    TokenType op;
    std::unique_ptr<astNode> operand;

public:
    UnaryOpNode(TokenType op, std::unique_ptr<astNode> operand)
            : op(op), operand(std::move(operand)) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Unary Operation: " << tokenTypeToString(op) << std::endl;
        operand->print(os, depth + 1);
    }

    std::string toJSON() const override {
        std::string json = "{ \"type\": \"UnaryOperation\", \"operator\": \"" + tokenTypeToString(op) +
                           "\", \"operand\": " + operand->toJSON() + " }";
        return json;
    }

};

// Function Call Nodes
class FunctionCallNode : public astNode {
    std::string funcName;
    std::vector<std::unique_ptr<astNode>> arguments;

public:
    FunctionCallNode(const std::string &funcName, std::vector<std::unique_ptr<astNode>> arguments = {})
            : funcName(funcName), arguments(std::move(arguments)) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Function Call: " << funcName << std::endl;
        for (const auto& arg : arguments) {
            arg->print(os, depth + 1);
        }
    }

    std::string toJSON() const override {
        std::string json = "{ \"type\": \"FunctionCall\", \"name\": \"" + funcName + "\", \"arguments\": [";
        for (size_t i = 0; i < arguments.size(); ++i) {
            json += arguments[i]->toJSON();
            if (i < arguments.size() - 1) json += ", ";
        }
        json += "] }";
        return json;
    }
};

// Function Declaration Nodes
class FunctionDeclarationNode : public astNode {
    std::string funcName;
    std::vector<std::pair<std::string, std::string>> parameters;  // pair of type and name
    std::vector<std::unique_ptr<astNode>> bodyStatements;  // A vector of statements

public:
    FunctionDeclarationNode(const std::string &funcName,
                            std::vector<std::pair<std::string, std::string>> parameters,
                            std::vector<std::unique_ptr<astNode>> bodyStatements)
            : funcName(funcName),
              parameters(std::move(parameters)),
              bodyStatements(std::move(bodyStatements)) {}


    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Function Declaration: " << funcName << std::endl;
        os << "Parameters: " << std::endl;
        for (const auto& param : parameters) {
            for (int j = 0; j < depth + 1; ++j) os << "--";
            os << param.first << " " << param.second << std::endl;
        }
        os << "Body:" << std::endl;
        for (const auto& stmt : bodyStatements) {
            stmt->print(os, depth + 1);
        }
    }

    std::string toJSON() const override {
        std::string json = "{ \"type\": \"FunctionDeclaration\", \"name\": \"" + funcName + "\", \"parameters\": [";
        for (size_t i = 0; i < parameters.size(); ++i) {
            json += "{ \"type\": \"" + parameters[i].first + "\", \"name\": \"" + parameters[i].second + "\" }";
            if (i < parameters.size() - 1) json += ", ";
        }
        json += "], \"body\": [";
        for (size_t i = 0; i < bodyStatements.size(); ++i) {
            json += bodyStatements[i]->toJSON();
            if (i < bodyStatements.size() - 1) json += ", ";
        }
        json += "] }";
        return json;
    }
};

#endif //TINYLANGUAGEPARSER_ASTNODE_H
