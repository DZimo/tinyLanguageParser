//
// Created by Admin on 8/13/2023.
//

#ifndef TINYLANGUAGEPARSER_ASTNODE_H
#define TINYLANGUAGEPARSER_ASTNODE_H


#include <memory>
#include <vector>
#include <sstream>
#include "../Lexical_checker/tokenizer.h"

class astNode {

public:
    virtual ~astNode() = default;  // Virtual destructor
    virtual std::string toJSON() const = 0;
    virtual void print(std::ostream& os, int depth = 0) const = 0;
};

class DeclarationNode : public astNode {
private:
    std::string dataType;
    std::string name;

public:
    DeclarationNode(const std::string &dataType, const std::string &name)
            : dataType(dataType), name(name) {}
    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Declaration: " << dataType << " " << name << std::endl;
    }
    std::string getDataType() const { return dataType; }
    std::string getName() const { return name; }
    std::string toJSON() const override {
        return "{ \"type\": \"Declaration\", \"name\": \"" + name + "\", \"dataType\": \"" + dataType + "\" }";
    }
};

class IdentifierNode : public astNode {
public:
    std::string value;

    IdentifierNode(const std::string& val) : value(val) {}

    std::string toJSON() const override {
        return "{\"type\":\"IdentifierNode\", \"value\":\"" + value + "\"}";
    }
    void printIndent(std::ostream& os, int depth) const {
        for (int i = 0; i < depth; ++i) {
            os << "  "; // Each depth level adds two spaces. Adjust this to your preference.
        }
    }
    void print(std::ostream& os, int depth = 0) const override {
        printIndent(os, depth);
        os << "Identifier: " << value << std::endl;
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

class MainFunctionNode : public astNode {
public:
    std::vector<std::unique_ptr<astNode>> declarations;
    std::vector<std::unique_ptr<astNode>> statements;

    MainFunctionNode(std::vector<std::unique_ptr<astNode>> decls,
                     std::vector<std::unique_ptr<astNode>> stmts)
            : declarations(std::move(decls)), statements(std::move(stmts)) {}

    std::string toJSON() const override {
        std::string result = "{";
        result += "\"type\":\"MainFunctionNode\", ";

        // Serialize declarations
        result += "\"declarations\":[";
        for (size_t i = 0; i < declarations.size(); ++i) {
            result += declarations[i]->toJSON();
            if (i < declarations.size() - 1) result += ",";
        }
        result += "], ";

        // Serialize statements
        result += "\"statements\":[";
        for (size_t i = 0; i < statements.size(); ++i) {
            result += statements[i]->toJSON();
            if (i < statements.size() - 1) result += ",";
        }
        result += "]";

        result += "}";
        return result;
    }

    void print(std::ostream& os, int depth = 0) const override {
        os << std::string(depth, ' ') << "MainFunction: \n";
        os << std::string(depth + 2, ' ') << "Declarations: " << '\n';
        for (const auto& decl : declarations) {
            decl->print(os, depth + 4);
        }
        os << std::string(depth + 2, ' ') << "Statements: " << '\n';
        for (const auto& stmt : statements) {
            stmt->print(os, depth + 4);
        }
    }
};

class FunctionNode : public astNode {
public:
    std::string returnType;
    std::string name;
    std::vector<std::pair<std::string, std::string>> parameters;
    std::vector<std::unique_ptr<astNode>> declarations;
    std::vector<std::unique_ptr<astNode>> statements;

    FunctionNode(const std::string& returnType,
                 const std::string& name,
                 std::vector<std::pair<std::string, std::string>> params,
                 std::vector<std::unique_ptr<astNode>> decls,
                 std::vector<std::unique_ptr<astNode>> stmts)
            : returnType(returnType), name(name), parameters(std::move(params)),
              declarations(std::move(decls)), statements(std::move(stmts)) {}

    std::string toJSON() const override {
        std::string result = "{";
        result += "\"type\":\"FunctionNode\", ";
        result += "\"returnType\":\"" + returnType + "\", ";
        result += "\"name\":\"" + name + "\", ";

        // Serialize parameters
        result += "\"parameters\":[";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += "{\"type\":\"" + parameters[i].first + "\",";
            result += "\"name\":\"" + parameters[i].second + "\"}";
            if (i < parameters.size() - 1) result += ",";
        }
        result += "], ";

        // Serialize declarations
        result += "\"declarations\":[";
        for (size_t i = 0; i < declarations.size(); ++i) {
            result += declarations[i]->toJSON();
            if (i < declarations.size() - 1) result += ",";
        }
        result += "], ";

        // Serialize statements
        result += "\"statements\":[";
        for (size_t i = 0; i < statements.size(); ++i) {
            result += statements[i]->toJSON();
            if (i < statements.size() - 1) result += ",";
        }
        result += "]";

        result += "}";
        return result;
    }

    void print(std::ostream& os, int depth = 0) const override {
        os << std::string(depth, ' ') << "Function: " << name << " returns " << returnType << '\n';
        for (const auto& param : parameters) {
            os << std::string(depth + 2, ' ') << "Param: " << param.first << " " << param.second << '\n';
        }
        os << std::string(depth + 2, ' ') << "Declarations: " << '\n';
        for (const auto& decl : declarations) {
            decl->print(os, depth + 4);
        }
        os << std::string(depth + 2, ' ') << "Statements: " << '\n';
        for (const auto& stmt : statements) {
            stmt->print(os, depth + 4);
        }
    }
};

class ConcatNode : public astNode {
public:
    std::unique_ptr<astNode> left;
    std::unique_ptr<astNode> right;

    ConcatNode(std::unique_ptr<astNode> l, std::unique_ptr<astNode> r)
            : left(std::move(l)), right(std::move(r)) {}

    std::string toJSON() const override {
        std::ostringstream oss;
        oss << "{ \"type\": \"ConcatNode\", \"left\": " << left->toJSON() << ", \"right\": " << right->toJSON() << " }";
        return oss.str();
    }

    void print(std::ostream& os, int depth = 0) const override {
        os << std::string(depth, ' ') << "Concat:\n";
        left->print(os, depth + 2);
        right->print(os, depth + 2);
    }
};

class AlternativeNode : public astNode {
public:
    std::unique_ptr<astNode> left;
    std::unique_ptr<astNode> right;

    AlternativeNode(std::unique_ptr<astNode> lhs, std::unique_ptr<astNode> rhs)
            : left(std::move(lhs)), right(std::move(rhs)) {}

    std::string toJSON() const override {
        std::ostringstream json;
        json << "{";
        json << "\"type\": \"AlternativeNode\",";
        json << "\"left\": " << left->toJSON() << ",";
        json << "\"right\": " << right->toJSON();
        json << "}";
        return json.str();
    }

    void print(std::ostream& os, int depth = 0) const override {
        os << std::string(depth * 2, ' ') << "AlternativeNode:\n";
        os << std::string((depth + 1) * 2, ' ') << "Left:\n";
        left->print(os, depth + 2);
        os << std::string((depth + 1) * 2, ' ') << "Right:\n";
        right->print(os, depth + 2);
    }
};

class CharacterNode : public astNode {
public:
    char value;

    explicit CharacterNode(char val) : value(val) {}

    std::string toJSON() const override {
        std::ostringstream json;
        json << "{";
        json << "\"type\": \"CharacterNode\",";
        json << "\"value\": \"" << value << "\"";
        json << "}";
        return json.str();
    }

    void print(std::ostream& os, int depth = 0) const override {
        os << std::string(depth * 2, ' ') << "CharacterNode: " << value << "\n";
    }
};

class assignementNode : public astNode {
private:
    std::unique_ptr<astNode> variable; // changed type to astNode for flexibility
    std::unique_ptr<astNode> expression;

public:
    assignementNode(std::unique_ptr<astNode> var, std::unique_ptr<astNode> expr)
            : variable(std::move(var)), expression(std::move(expr)) {}

    // Getter methods for accessing the variable and expression.
    const astNode& getVariable() const {
        return *variable;
    }

    const astNode& getExpression() const {
        return *expression;
    }

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Assignment:" << std::endl;

        variable->print(os, depth + 1);  // Print the variable
        expression->print(os, depth + 1);     // Print the expression
    }

    std::string toJSON() const override {
        return "{ \"type\": \"Assignment\", \"left\": " + variable->toJSON() + ", \"right\": " + expression->toJSON() + " }";
    }
};

class RuleNode : public astNode {
public:
    std::string ruleName;
    std::unique_ptr<astNode> body;

    RuleNode(const std::string& name, std::unique_ptr<astNode> ruleBody)
            : ruleName(name), body(std::move(ruleBody)) {}

    std::string toJSON() const override {
        return "{\"type\":\"RuleNode\", \"name\":\"" + ruleName + "\", \"body\":" + body->toJSON() + "}";
    }

    void printIndent(std::ostream& os, int depth) const {
        for (int i = 0; i < depth; ++i) {
            os << "  "; // Each depth level adds two spaces. You can adjust this to your preference.
        }
    }
    void print(std::ostream& os, int depth = 0) const override {
        printIndent(os, depth);
        os << "Rule: " << ruleName << std::endl;
        body->print(os, depth + 1);
    }


};

#endif //TINYLANGUAGEPARSER_ASTNODE_H
