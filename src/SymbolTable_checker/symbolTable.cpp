//
// Created by Admin on 8/11/2023.
//

#include "symbolTable.h"
#include "string.h"
#include "../Lexical_checker/tokenizer.cpp"
#include <iostream>
#include <unordered_map>

class symbol {
public:
    std::string name;
    TokenType type;
    // Other attributes can be added as necessary, such as datatype, scope, memory address, etc.
    symbol() {}
    symbol(const std::string& name, TokenType type): name(name), type(type) {}
};

class symbolTable {
private:
    std::unordered_map<std::string, symbol> table;
public:
    void insert(const std::string& name, TokenType type) {
        table.insert(std::make_pair(name, symbol(name, type)));
    }

    bool exists(const std::string& name) {
        return table.find(name) != table.end();
    }

    symbol get(const std::string& name) {
        if (exists(name)) {
            return table[name];
        }
        throw std::runtime_error("Invalid Program : Symbol not found in the table!");
    }

    void print() const {
        for (const auto &entry : table) {
            std::cout << "Identifier: " << entry.first
                      << ", Type: " << tokenTypeToString(entry.second.type)
                      << std::endl;
        }
    }
};
