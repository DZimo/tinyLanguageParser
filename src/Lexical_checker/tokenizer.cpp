//
// Created by Admin on 8/10/2023.
//

#include "tokenizer.h"


class tokenizer {
private:
    int line_number;
    int char_position;
public:
    // Constructor
    tokenizer(TokenType type, const std::string& value, int line_number, int char_position)
            : type(type), value(value), line_number(line_number), char_position(char_position) {}

    // Getter methods
    TokenType getType() const {
        return type;
    }

    const std::string& getValue() const {
        return value;
    }

    int getLineNumber() const {
        return line_number;
    }

    int getCharPosition() const {
        return char_position;
    }

    // Debug method to print the token
    void print() const {
        std::cout << "Token(Type: " << tokenTypeToString(type)
                  << ", Value: " << value
                  << ", Line: " << line_number
                  << ", Char: " << char_position
                  << ")" << std::endl;
    }

    TokenType type;
    std::string value;
};