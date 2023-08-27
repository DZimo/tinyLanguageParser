
#include "symbolTable.h"

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
    std::vector<std::unordered_map<std::string, astNode*>> scopes;

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

    void pushScope() {
        scopes.emplace_back();
    }

    void popScope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }

    bool insertSymbol(const std::string &name, astNode* node) {
        if (scopes.empty()) return false;
        auto &currentScope = scopes.back();
        // Avoid duplicate insertions in the same scope
        if (currentScope.find(name) != currentScope.end()) {
            return false;
        }
        currentScope[name] = node;
        return true;
    }

    astNode* lookupSymbol(const std::string &name) {
        // Traverse scopes from innermost to outermost
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            if (it->find(name) != it->end()) {
                return (*it)[name];
            }
        }
        return nullptr; // Symbol not found
    }
};
