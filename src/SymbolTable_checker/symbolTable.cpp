
#include "symbolTable.h"

class symbol {
public:
    std::string name;
    TokenType type;
    int value;  // Add this to store the value (assuming it's an integer)

    // Other attributes can be added as necessary, such as datatype, scope, memory address, etc.
    symbol() {}
    symbol(const std::string& name, TokenType type, int value = 0)
            : name(name), type(type), value(value) {}
};

class symbolTable {
private:
    std::unordered_map<std::string, symbol> table;
    std::vector<std::unordered_map<std::string, astNode*>> scopes;
    bool isLocked = false;

public:
    void pushLockedScope() {
        if (isLocked) {
            throw std::runtime_error("A locked scope is already in place. Can't push another locked scope.");
        }
        scopes.emplace_back();
        isLocked = true;  // Lock this scope
    }

    void insertGlobalSymbol(const std::string &name, astNode* node) {
        if (scopes.empty()) {
            throw std::runtime_error("No global scope found.");
        }
        scopes[0][name] = node;  // Always insert into the global scope (the first scope)
    }

    void insert(const std::string& name, TokenType type) {
        table.insert(std::make_pair(name, symbol(name, type)));
    }

    // Function to update the value of an existing symbol
    void updateValue(const std::string& name, int value) {
        if (exists(name)) {
            table[name].value = value;
        } else {
            throw std::runtime_error("Symbol not found in the table!");
        }
    }

    // Function to get the value of an existing symbol
    int getValue(const std::string& name) {
        if (exists(name)) {
            return table[name].value;
        } else {
            throw std::runtime_error("Symbol not found in the table!");
        }
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
    /*
    bool insertSymbol(const std::string &name, astNode* node) {
        if (scopes.empty()) return false;
        auto &currentScope = scopes.back();
        // Avoid duplicate insertions in the same scope
        if (currentScope.find(name) != currentScope.end()) {
            return false;
        }
        currentScope[name] = node;
        return true;
    }*/

    bool insertSymbol(const std::string &name, astNode* node) {
        if (scopes.empty()) {
            return false; // No scope to insert into
        }

        auto &currentScope = scopes.back();
        // Insert only if the name does not already exist in the current scope
        if (currentScope.find(name) == currentScope.end()) {
            currentScope[name] = node;
            return true; // Successfully inserted
        } else {
            return false; // Name already exists in the current scope
        }
    }

    void updateValueInScopes(const std::string& name, std::unique_ptr<astNode> newValue) {
        // Start from the innermost scope and search outwards
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                astNode* node = found->second;

                // Dynamic cast to DeclarationNode
                DeclarationNode* declNode = dynamic_cast<DeclarationNode*>(node);
                if (declNode) {
                    declNode->updateValue(std::move(newValue));
                    return; // Successfully updated, exit the function
                }
            }
        }
        throw std::runtime_error("Symbol not found in any scope");
    }

    astNode* lookupSymbol(const std::string &name) {
        // Start from the innermost scope and search outwards
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second; // Return the node if found
            }
        }
        return nullptr; // Symbol not found in any scope
    }

    /*
    astNode* lookupSymbol(const std::string &name) {
        // Traverse scopes from innermost to outermost
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            if (it->find(name) != it->end()) {
                return (*it)[name];
            }
        }
        return nullptr; // Symbol not found
    }*/
};
