//
// Created by Admin on 8/10/2023.
//

#ifndef TINYLANGUAGEPARSER_TOKENIZER_H
#define TINYLANGUAGEPARSER_TOKENIZER_H
#include <string>
#include <iostream>

enum class TokenType {
    INT, FLOAT, CHAR, BOOL,       // Types
    IDENTIFIER,                   // Variables and function names
    NUMBER,                       // Numerical constants (this can be further split into INT_NUMBER and FLOAT_NUMBER if needed)
    CHAR_VALUE,                   // Character constants
    ADD_OP, SUB_OP, MUL_OP, DIV_OP, MOD_OP, // Arithmetic operators
    AND_OP, OR_OP, NOT_OP,        // Logical operators
    EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, // Relational operators
    ASSIGNMENT,                   // Assignment operator
    L_PAREN, R_PAREN,             // Parentheses
    L_BRACE, R_BRACE,             // Curly braces
    COMMA, SEMICOLON,             // Punctuation
    IF, ELSE, WHILE, RETURN,          // Keywords
    EOF_TOK,INCREMENT,                       // End of file/token stream
    LOGICAL_AND, LOGICAL_OR,
    UNKNOWN,    MAIN,
};

enum class EBNFtokenType {
    RULE_START,     // <
    RULE_END,       // >
    ASSIGNMENT,     // ::=
    LITERAL_STRING, // "int", "main", "if", and so on...
    OR_OP,          // |
    L_BRACE,        // {
    R_BRACE,        // }
    L_SQUARE,       // [
    R_SQUARE,       // ]
    L_PAREN,        // (
    R_PAREN,        // )
    SEMICOLON,      // ;
    COMMA,          // ,
    IDENTIFIER,     // Matches [a-zA-Z_][a-zA-Z0-9_]*
    NUMBER,         // Matches both int_value and float_value
    CHAR_VALUE,     // Matches 'char'
    EOF_TOK         // End of file/token stream for EBNF
};

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INT: return "INT";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::CHAR: return "CHAR";
        case TokenType::BOOL: return "BOOL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::CHAR_VALUE: return "CHAR_VALUE";
        case TokenType::ADD_OP: return "ADD_OP";
        case TokenType::SUB_OP: return "SUB_OP";
        case TokenType::MUL_OP: return "MUL_OP";
        case TokenType::DIV_OP: return "DIV_OP";
        case TokenType::MOD_OP: return "MOD_OP";
        case TokenType::AND_OP: return "AND_OP";
        case TokenType::OR_OP: return "OR_OP";
        case TokenType::NOT_OP: return "NOT_OP";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LESS_THAN: return "LESS_THAN";
        case TokenType::GREATER_THAN: return "GREATER_THAN";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::ASSIGNMENT: return "ASSIGNMENT";
        case TokenType::L_PAREN: return "L_PAREN";
        case TokenType::R_PAREN: return "R_PAREN";
        case TokenType::L_BRACE: return "L_BRACE";
        case TokenType::R_BRACE: return "R_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::EOF_TOK: return "EOF_TOK";
        default: return "UNKNOWN";
    }
}

#endif //TINYLANGUAGEPARSER_TOKENIZER_H
