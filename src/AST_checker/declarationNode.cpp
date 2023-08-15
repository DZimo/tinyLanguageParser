//
// Created by Admin on 8/13/2023.
//

#include "declarationNode.h"
#include "astNode.h"

class declarationNode : public astNode {
public:
    std::string type;
    std::string name;

    declarationNode(const std::string& type, const std::string& name) : type(type), name(name) {}

    void print(std::ostream& os, int depth = 0) const override {
        for (int i = 0; i < depth; ++i) os << "--";
        os << "Declaration: " << type << " " << name << std::endl;
    }
};