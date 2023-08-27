/*#ifndef TINYLANGUAGEPARSER_EBNFLEXER_H
#define TINYLANGUAGEPARSER_EBNFLEXER_H
#pragma once
#include "tokenizer.h";

class EBNFtokenizer {
private:
    int line_number;
    int char_position;

public:
    EBNFtokenizer(EBNFtokenType type, const std::string& value, int line, int char_pos)
            : type(type), value(value), line_number(line), char_position(char_pos) {}

    EBNFtokenType getType() const { return type; }
    std::string getValue() const { return value; }
    int getLineNumber() const { return line_number; }
    int getCharPosition() const { return char_position; }

    EBNFtokenType type;
    std::string value;
};

class EBNFlexer {
private:
    std::string source;
    int pos;
    char current_char;
    int line_number;
    int char_position;

    void advance() {
        pos++;
        if (pos >= source.length()) {
            current_char = EOF;
        } else {
            current_char = source[pos];
            char_position++;
        }
    }

    void skipWhitespace() {
        while (current_char != EOF && std::isspace(current_char)) {
            if (current_char == '\n') {
                line_number++;
                char_position = 1;
            } else {
                char_position++;
            }
            advance();
        }
    }

    EBNFtokenizer getIdentifier() {
        int start_pos = pos;
        while (current_char != EOF && (std::isalnum(current_char) || current_char == '_')) {
            advance();
        }
        std::string value = source.substr(start_pos, pos - start_pos);
        return EBNFtokenizer(EBNFtokenType::IDENTIFIER, value, line_number, char_position);
    }

    EBNFtokenizer getLiteralString() {
        advance();  // Consume the opening quote
        int start_pos = pos;
        while (current_char != '"' && current_char != EOF) {
            advance();
        }
        std::string value = source.substr(start_pos, pos - start_pos);
        advance();  // Consume the closing quote
        return EBNFtokenizer(EBNFtokenType::LITERAL_STRING, value, line_number, char_position);
    }

public:
    EBNFlexer(const std::string& src) : source(src), pos(0), current_char(src[0]), line_number(1), char_position(1) {}

    EBNFtokenizer getNextToken() {
        while (current_char != EOF) {
            skipWhitespace();

            if (std::isalpha(current_char) || current_char == '_') {
                return getIdentifier();
            }

            switch (current_char) {
                case '<':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::RULE_START, "<", line_number, char_position);
                case '>':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::RULE_END, ">", line_number, char_position);
                case ':':
                    advance();
                    if (current_char == ':') {
                        advance();
                        if (current_char == '=') {
                            advance();
                            return EBNFtokenizer(EBNFtokenType::ASSIGNMENT, "::=", line_number, char_position - 2);
                        }
                    }
                    break;
                case '|':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::OR_OP, "|", line_number, char_position);
                case '{':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::L_BRACE, "{", line_number, char_position);
                case '}':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::R_BRACE, "}", line_number, char_position);
                case '[':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::L_SQUARE, "[", line_number, char_position);
                case ']':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::R_SQUARE, "]", line_number, char_position);
                case '(':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::L_PAREN, "(", line_number, char_position);
                case ')':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::R_PAREN, ")", line_number, char_position);
                case ';':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::SEMICOLON, ";", line_number, char_position);
                case ',':
                    advance();
                    return EBNFtokenizer(EBNFtokenType::COMMA, ",", line_number, char_position);
                case '"':
                    return getLiteralString();
                default:
                    advance();
            }
        }

        return EBNFtokenizer(EBNFtokenType::EOF_TOK, "", line_number, char_position);
    }
};


#endif //TINYLANGUAGEPARSER_EBNFLEXER_H
*/