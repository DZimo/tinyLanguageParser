//
// Created by Admin on 8/15/2023.
//

#ifndef TINYLANGUAGEPARSER_EBNFCHECKER_H
#define TINYLANGUAGEPARSER_EBNFCHECKER_H
#include "../AST_checker/astNode.h"
/*
class EBNFchecker {
    class EBNFChecker {
        astNode* currentNode;

    public:
        explicit EBNFChecker(astNode* root) : currentNode(root) {}

        void visit(astNode* node) {
            if (dynamic_cast<FunctionDeclarationNode*>(node)) {
                validateFunctionDefinition();
            }
            // add other type checks here for other nodes

            for (const auto& child : node->children) {
                visit(child.get());
            }
        }

    private:
        void validateFunctionDefinition() {
            // Check type
            if (!dynamic_cast<TypeNode*>(currentNode->children[0].get())) {
                throw std::runtime_error("Expected a type at the start of a function definition.");
            }

            // Check identifier
            if (!dynamic_cast<IdentifierNode*>(currentNode->children[1].get())) {
                throw std::runtime_error("Expected an identifier after the type in a function definition.");
            }

            // Check for opening parenthesis
            // ... similar checks for other components of a function definition

            // You would recursively apply this logic to match each part of your EBNF rules.
        }

        // ... additional validation functions for other EBNF rules ...
    };
};
*/

#endif //TINYLANGUAGEPARSER_EBNFCHECKER_H
