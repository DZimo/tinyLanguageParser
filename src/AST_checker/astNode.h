
#ifndef TINYLANGUAGEPARSER_ASTNODE_H
#define TINYLANGUAGEPARSER_ASTNODE_H

#include "../headersGeneralizer.h"
#include "../Lexical_checker/tokenizer.h"



class astNode {

public:
    virtual ~astNode() = default;

    std::string toJSON() const {
            if(this != NULL)
            {
                std::ostringstream json;
                json << "{ \"type\": \"" << getType() << "\", ";
                appendToJSON(json);
                json << " }";
                return json.str();
            }
        return "";
    }

    void print(std::ostream& os, int depth = 0) const {
        for (int i = 0; i < depth; ++i) os << "--";
        os << getDescription() << std::endl;
        printChildren(os, depth + 1);
    }

    int getMaxDepthRecursive() const {
        int maxChildDepth = getChildrenDepth();
        return 1 + maxChildDepth;  // 1 for the current node + maximum depth of children
    }


public:
    // Override these in derived classes
    virtual std::string getType() const = 0;
    virtual void appendToJSON(std::ostringstream& os) const = 0;
    virtual std::string getDescription() const = 0;
    virtual void printChildren(std::ostream& os, int depth) const {}
    virtual const std::vector<std::unique_ptr<astNode>>& getChildren() const {
        static const std::vector<std::unique_ptr<astNode>> emptyChildren = {};
        return emptyChildren;  // By default, return a reference to an empty vector
    }
    virtual int getChildrenDepth() const { return 0; }  // Default implementation returns 0. This needs to be overridden in derived classes.
    /*virtual int evaluate( const symbolTable& table ) const {
        throw std::runtime_error("evaluate not implemented for node of type " + getType());
    }*/

};

class DeclarationNode : public astNode {
public:
    std::string dataType;
    std::string name;
    std::unique_ptr<astNode> value;  // New field to hold the value

protected:
    std::string getType() const override { return "Declaration"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"name\": \"" << name << "\", \"dataType\": \"" << dataType << "\"";
    }
    std::string getDescription() const override {
        return "Declaration: " + dataType + " " + name;
    }
    int getChildrenDepth() const override {
        return 0;  // DeclarationNode does not have children, so it returns 0.
    }

public:
    DeclarationNode(const std::string &dataType, const std::string &name)
            : dataType(dataType), name(name) {}

    void updateValue(std::unique_ptr<astNode> newValue) {
        value = std::move(newValue);
    }
    // Getter for the value, can be useful in other parts of the code
    astNode* getValue() const {
        return value.get();
    }
};

class ArrayDeclarationNode : public astNode {
private:
    std::string dataType;
    std::string name;
    int size;

protected:
    std::string getType() const override { return "ArrayDeclaration"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"name\": \"" << name << "\", \"dataType\": \"" << dataType << "\", \"size\": " << size;
    }
    std::string getDescription() const override {
        return "ArrayDeclaration: " + dataType + " " + name + "[" + std::to_string(size) + "]";
    }
    int getChildrenDepth() const override {
        return 0;  // ArrayDeclarationNode does not have children, so it returns 0.
    }
public:
    ArrayDeclarationNode(const std::string &dataType, const std::string &name, int size)
            : dataType(dataType), name(name), size(size) {}
};

class CompoundNode : public astNode {
public:
    std::vector<std::unique_ptr<astNode>> nodes;

protected:
    std::string getType() const override {
        return "Compound";
    }
    const std::vector<std::unique_ptr<astNode>>& getChildren() const override {
        return nodes;  // Return a reference to the nodes for CompoundNode
    }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"type\": \"" << getType() << "\", ";
        os << "\"nodes\": [";
        for (size_t i = 0; i < nodes.size(); ++i) {
            os << nodes[i]->toJSON();
            if (i != nodes.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
    }

    std::string getDescription() const override {
        std::string description = "Compound Node with children: ";
        for (const auto& node : nodes) {
           // description += "\n-> " + node->getDescription();
        }
        return description;
    }

    int getChildrenDepth() const override {
        int maxDepth = 0;
        for (const auto& child : nodes) {
            maxDepth = std::max(maxDepth, child->getMaxDepthRecursive());
        }
        return maxDepth;
    }

public:
    CompoundNode(std::vector<std::unique_ptr<astNode>> nodes)
            : nodes(std::move(nodes)) {}
};

class IdentifierNode : public astNode {
public:
    std::string value;

protected:
    std::string getType() const override { return "IdentifierNode"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"value\": \"" << value << "\"";
    }
    std::string getDescription() const override {
        return "Identifier: " + value;
    }
    int getChildrenDepth() const override {
        return 0;  // IdentifierNode does not have children, so it returns 0.
    }
public:
    IdentifierNode(const std::string& val) : value(val) {}
};

class BinaryOpNode : public astNode {
public:
    std::unique_ptr<astNode> left;
    std::unique_ptr<astNode> right;
    TokenType op;

protected:
    std::string getType() const override { return "BinaryExpression"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"operator\": \"" << tokenTypeToString(op) << "\", "
           << "\"left\": " << left->toJSON() << ", \"right\": " << right->toJSON();
    }
    std::string getDescription() const override {
        return "BinaryOp: " + tokenTypeToString(op);
    }
    void printChildren(std::ostream& os, int depth) const override {
        left->print(os, depth);
        right->print(os, depth);
    }
    int getChildrenDepth() const override {
        int leftDepth = left ? left->getMaxDepthRecursive() : 0;
        int rightDepth = right ? right->getMaxDepthRecursive() : 0;
        return std::max(leftDepth, rightDepth);
    }
public:

    BinaryOpNode(std::unique_ptr<astNode> left, TokenType op, std::unique_ptr<astNode> right)
            : left(std::move(left)), op(op), right(std::move(right)) {}
/*
    int evaluate(const symbolTable& table) const {
        int leftVal = left->evaluate(table);
        int rightVal = right->evaluate(table);
        if (op == TokenType::ADD_OP) {
            return leftVal + rightVal;
        } else if (op == TokenType::SUB_OP) {
            return leftVal - rightVal;
        } else if (op == TokenType::MUL_OP) {
            return leftVal * rightVal;
        } else if (op == TokenType::DIV_OP) {
            if (rightVal == 0) {
                throw std::runtime_error("Division by zero");
            }
            return leftVal / rightVal;
        } else {
            throw std::runtime_error("Unknown operator: " + tokenTypeToString(op));
        }
    }*/
};

class NumberNode : public astNode {
public:
    std::string value;

protected:
    std::string getType() const override { return "Number"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"value\": \"" << value << "\"";
    }
    std::string getDescription() const override {
        return "Number: " + value;
    }
    int getChildrenDepth() const override {
        return 0;  // NumberNode does not have children, so it returns 0.
    }

public:
    explicit NumberNode(const std::string& value) : value(value) {}
    /*int evaluate(const symbolTable& table) const override {
        try {
            return std::stoi(value);
        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Invalid number: " + value);
        } catch (const std::out_of_range& e) {
            throw std::runtime_error("Number out of range: " + value);
        }
    }*/
};

enum class Operator {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
};

class VariableNode : public astNode {
public:
    std::string name;
protected:
    std::string getType() const override { return "Variable"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"name\": \"" << name << "\"";
    }
    std::string getDescription() const override {
        return "Variable: " + name;
    }

public:
    explicit VariableNode(const std::string& name)
            : name(name) {}
    /*int evaluate(const symbolTable& table) const override {
        astNode* node = table.lookupSymbol(name);
        if (node) {
            // Assuming that the node returned can be evaluated to an integer
            // You may want to add more checks here depending on your needs
            return node->evaluate(table);
        } else {
            throw std::runtime_error("Variable not found: " + name);
        }
    }*/
};

// Block Statements
class BlockNode : public astNode {
public:
    std::vector<std::unique_ptr<astNode>> statements;

protected:
    std::string getType() const override { return "Block"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"statements\": [";
        for (size_t i = 0; i < statements.size(); ++i) {
            os << statements[i]->toJSON();
            if (i < statements.size() - 1) os << ", ";
        }
        os << "]";
    }
    std::string getDescription() const override {
        return "Block:";
    }
    void printChildren(std::ostream& os, int depth) const override {
        for (const auto& stmt : statements) {
            stmt->print(os, depth);
        }
    }

public:
    explicit BlockNode(std::vector<std::unique_ptr<astNode>> statements)
            : statements(std::move(statements)) {}
};

// If Statements
class IfNode : public astNode {
public:
    std::unique_ptr<astNode> condition;
    std::unique_ptr<astNode> trueBranch;
    std::unique_ptr<astNode> falseBranch;

protected:
    std::string getType() const override { return "IfStatement"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"condition\": " << condition->toJSON()
           << ", \"trueBranch\": " << trueBranch->toJSON();
        if (falseBranch) {
            os << ", \"falseBranch\": " << falseBranch->toJSON();
        }
    }
    std::string getDescription() const override {
        return "If Condition:";
    }
    void printChildren(std::ostream& os, int depth) const override {
        condition->print(os, depth);
        trueBranch->print(os, depth);
        if (falseBranch) {
            os << std::string(depth, ' ') << "False Branch:\n";
            falseBranch->print(os, depth);
        }
    }

public:
    IfNode(std::unique_ptr<astNode> condition, std::unique_ptr<astNode> trueBranch, std::unique_ptr<astNode> falseBranch = nullptr)
            : condition(std::move(condition)), trueBranch(std::move(trueBranch)), falseBranch(std::move(falseBranch)) {}
};

// While Statements
class WhileNode : public astNode {
public:
    std::unique_ptr<astNode> condition;
    std::unique_ptr<astNode> loopBody;

protected:
    std::string getType() const override { return "WhileLoop"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"condition\": " << condition->toJSON()
           << ", \"body\": " << loopBody->toJSON();
    }
    std::string getDescription() const override {
        return "While Condition:";
    }
    void printChildren(std::ostream& os, int depth) const override {
        condition->print(os, depth);
        loopBody->print(os, depth);
    }

public:
    WhileNode(std::unique_ptr<astNode> condition, std::unique_ptr<astNode> loopBody)
            : condition(std::move(condition)), loopBody(std::move(loopBody)) {}
};

// Unary Operation Nodes

class UnaryOpNode : public astNode {
public:
    TokenType op;
    std::unique_ptr<astNode> operand;

protected:
    std::string getType() const override { return "UnaryOperation"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"operator\": \"" << tokenTypeToString(op) << "\", "
           << "\"operand\": " << operand->toJSON();
    }
    std::string getDescription() const override {
        return "Unary Operation: " + tokenTypeToString(op);
    }
    void printChildren(std::ostream& os, int depth) const override {
        operand->print(os, depth);
    }

public:
    UnaryOpNode(TokenType op, std::unique_ptr<astNode> operand)
            : op(op), operand(std::move(operand)) {}
};

// Function Call Nodes
class FunctionCallNode : public astNode {
public:
    std::string funcName;
    std::vector<std::unique_ptr<astNode>> arguments;

protected:
    std::string getType() const override { return "FunctionCall"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"name\": \"" << funcName << "\", \"arguments\": [";
        for (size_t i = 0; i < arguments.size(); ++i) {
            os << arguments[i]->toJSON();
            if (i < arguments.size() - 1) os << ", ";
        }
        os << "]";
    }
    std::string getDescription() const override {
        return "Function Call: " + funcName;
    }
    void printChildren(std::ostream& os, int depth) const override {
        for (const auto& arg : arguments) {
            arg->print(os, depth);
        }
    }

public:
    FunctionCallNode(const std::string &funcName, std::vector<std::unique_ptr<astNode>> arguments = {})
            : funcName(funcName), arguments(std::move(arguments)) {}
};

// Function Declaration Nodes
class FunctionDeclarationNode : public astNode {
public:
    std::string funcName;
    std::vector<std::pair<std::string, std::string>> parameters;
    std::vector<std::unique_ptr<astNode>> bodyStatements;

protected:
    std::string getType() const override { return "FunctionDeclaration"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"name\": \"" << funcName << "\", \"parameters\": [";
        for (size_t i = 0; i < parameters.size(); ++i) {
            os << "{ \"type\": \"" << parameters[i].first << "\", \"name\": \"" << parameters[i].second << "\" }";
            if (i < parameters.size() - 1) os << ", ";
        }
        os << "], \"body\": [";
        for (size_t i = 0; i < bodyStatements.size(); ++i) {
            os << bodyStatements[i]->toJSON();
            if (i < bodyStatements.size() - 1) os << ", ";
        }
        os << "]";
    }
    std::string getDescription() const override {
        return "Function Declaration: " + funcName;
    }
    void printChildren(std::ostream& os, int depth) const override {
        for (const auto& stmt : bodyStatements) {
            stmt->print(os, depth);
        }
    }

public:
    FunctionDeclarationNode(const std::string &funcName,
                            std::vector<std::pair<std::string, std::string>> parameters,
                            std::vector<std::unique_ptr<astNode>> bodyStatements)
            : funcName(funcName),
              parameters(std::move(parameters)),
              bodyStatements(std::move(bodyStatements)) {}
};

class ReturnNode : public astNode {
public:
    std::unique_ptr<astNode> returnValue;

    ReturnNode(std::unique_ptr<astNode> returnValue) : returnValue(std::move(returnValue)) {}

protected:
    std::string getType() const override { return "Return"; }

    void appendToJSON(std::ostringstream& os) const override {
        os << "\"return_value\": " << returnValue->toJSON();
    }

    std::string getDescription() const override {
        return "Return Statement";
    }

    void printChildren(std::ostream& os, int depth) const override {
        returnValue->print(os, depth);
    }
};

class MainFunctionNode : public astNode {
public:
    std::vector<std::unique_ptr<astNode>> declarations;
    std::vector<std::unique_ptr<astNode>> statements;

protected:
    std::string getType() const override { return "MainFunctionNode"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"declarations\": [";
        for (size_t i = 0; i < declarations.size(); ++i) {
            os << declarations[i]->toJSON();
            if (i < declarations.size() - 1) os << ", ";
        }
        os << "], \"statements\": [";
        for (size_t i = 0; i < statements.size(); ++i) {
            os << statements[i]->toJSON();
            if (i < statements.size() - 1) os << ", ";
        }
        os << "]";
    }
    std::string getDescription() const override {
        return "MainFunction:";
    }
    void printChildren(std::ostream& os, int depth) const override {
        for (const auto& decl : declarations) {
            decl->print(os, depth);
        }
        for (const auto& stmt : statements) {
            stmt->print(os, depth);
        }
    }

public:
    MainFunctionNode(std::vector<std::unique_ptr<astNode>> decls,
                     std::vector<std::unique_ptr<astNode>> stmts)
            : declarations(std::move(decls)), statements(std::move(stmts)) {}
};

class FunctionNode : public astNode {
public:
    std::string returnType;
    std::string name;
    std::vector<std::pair<std::string, std::string>> parameters;  // pair of type and name
    std::vector<std::unique_ptr<astNode>> declarations;
    std::vector<std::unique_ptr<astNode>> statements;

protected:
    std::string getType() const override { return "FunctionNode"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"returnType\": \"" << returnType << "\", ";
        os << "\"name\": \"" << name << "\", ";

        // Serialize parameters
        os << "\"parameters\": [";
        for (size_t i = 0; i < parameters.size(); ++i) {
            os << "{\"type\": \"" << parameters[i].first << "\", \"name\": \"" << parameters[i].second << "\"}";
            if (i < parameters.size() - 1) os << ", ";
        }
        os << "], ";

        // Serialize declarations
        os << "\"declarations\": [";
        for (size_t i = 0; i < declarations.size(); ++i) {
            os << declarations[i]->toJSON();
            if (i < declarations.size() - 1) os << ", ";
        }
        os << "], ";

        // Serialize statements
        os << "\"statements\": [";
        for (size_t i = 0; i < statements.size(); ++i) {
            os << statements[i]->toJSON();
            if (i < statements.size() - 1) os << ", ";
        }
        os << "]";
    }

    std::string getDescription() const override {
        return "Function: " + name + " returns " + returnType;
    }

    void printChildren(std::ostream& os, int depth) const override {
        for (const auto& param : parameters) {
            os << std::string(depth, ' ') << "Param: " << param.first << " " << param.second << '\n';
        }
        os << std::string(depth, ' ') << "Declarations: " << '\n';
        for (const auto& decl : declarations) {
            decl->print(os, depth + 2);
        }
        os << std::string(depth, ' ') << "Statements: " << '\n';
        for (const auto& stmt : statements) {
            stmt->print(os, depth + 2);
        }
    }

public:
    FunctionNode(const std::string& returnType,
                 const std::string& name,
                 std::vector<std::pair<std::string, std::string>> params,
                 std::vector<std::unique_ptr<astNode>> decls,
                 std::vector<std::unique_ptr<astNode>> stmts)
            : returnType(returnType), name(name), parameters(std::move(params)),
              declarations(std::move(decls)), statements(std::move(stmts)) {}
};

class ConcatNode : public astNode {
public:
    std::unique_ptr<astNode> left;
    std::unique_ptr<astNode> right;

protected:
    std::string getType() const override { return "ConcatNode"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"left\": " << left->toJSON() << ", \"right\": " << right->toJSON();
    }
    std::string getDescription() const override {
        return "Concat:";
    }
    void printChildren(std::ostream& os, int depth) const override {
        left->print(os, depth);
        right->print(os, depth);
    }

public:
    ConcatNode(std::unique_ptr<astNode> l, std::unique_ptr<astNode> r)
            : left(std::move(l)), right(std::move(r)) {}
};

class AlternativeNode : public astNode {
public:
    std::unique_ptr<astNode> left;
    std::unique_ptr<astNode> right;

protected:
    std::string getType() const override { return "AlternativeNode"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"left\": " << left->toJSON() << ", \"right\": " << right->toJSON();
    }
    std::string getDescription() const override {
        return "AlternativeNode:";
    }
    void printChildren(std::ostream& os, int depth) const override {
        left->print(os, depth);
        right->print(os, depth);
    }

public:
    AlternativeNode(std::unique_ptr<astNode> lhs, std::unique_ptr<astNode> rhs)
            : left(std::move(lhs)), right(std::move(rhs)) {}
};

class CharacterNode : public astNode {
public:
    char value;

protected:
    std::string getType() const override { return "CharacterNode"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"value\": \"" << value << "\"";
    }
    std::string getDescription() const override {
        return "CharacterNode: " + std::string(1, value);
    }

public:
    explicit CharacterNode(char val) : value(val) {}
};

class AssignmentNode : public astNode {
public:
    std::unique_ptr<astNode> variable;
    std::unique_ptr<astNode> expression;

protected:
    std::string getType() const override { return "Assignment"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"left\": " << variable->toJSON() << ", \"right\": " << expression->toJSON();
    }
    std::string getDescription() const override {
        return "Assignment:";
    }
    void printChildren(std::ostream& os, int depth) const override {
        variable->print(os, depth);
        expression->print(os, depth);
    }

public:
    AssignmentNode(std::unique_ptr<astNode> var, std::unique_ptr<astNode> expr)
            : variable(std::move(var)), expression(std::move(expr)) {}
};

class RuleNode : public astNode {
public:
    std::string ruleName;
    std::unique_ptr<astNode> body;

protected:
    std::string getType() const override { return "RuleNode"; }
    void appendToJSON(std::ostringstream& os) const override {
        os << "\"name\": \"" << ruleName << "\", \"body\": " << body->toJSON();
    }
    std::string getDescription() const override {
        return "Rule: " + ruleName;
    }
    void printChildren(std::ostream& os, int depth) const override {
        body->print(os, depth);
    }

public:
    RuleNode(const std::string& name, std::unique_ptr<astNode> ruleBody)
            : ruleName(name), body(std::move(ruleBody)) {}
};

class BooleanNode : public astNode {
private:
    bool value;

public:
    // Constructor
    BooleanNode(bool val) : value(val) {}

protected:
    std::string getType() const override {
        return "Boolean";
    }

    void appendToJSON(std::ostringstream& os) const override {
        os << "\"value\": " << (value ? "true" : "false");
    }

    std::string getDescription() const override {
        return "Boolean Literal";
    }

    // Since a BooleanNode won't have children nodes, you can override the printChildren method to do nothing.
    void printChildren(std::ostream& os, int depth) const override {}
};

#endif //TINYLANGUAGEPARSER_ASTNODE_H
