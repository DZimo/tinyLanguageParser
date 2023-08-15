//
// Created by Admin on 8/13/2023.
//

#ifndef TINYLANGUAGEPARSER_VARIABLENODE_H
#define TINYLANGUAGEPARSER_VARIABLENODE_H
#include "astNode.h"


class variableNode : public astNode {
private:
    std::string identifier;

public:
    explicit variableNode(const std::string& id)
            : identifier(id) {}

    const std::string& getIdentifier() const {
        return identifier;
    }
};

#endif //TINYLANGUAGEPARSER_VARIABLENODE_H
