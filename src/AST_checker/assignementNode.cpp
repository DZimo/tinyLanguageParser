//
// Created by Admin on 8/13/2023.
//

#include <memory>
#include "assignementNode.h"
#include "astNode.h"



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