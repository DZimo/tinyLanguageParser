#include "lexer.h"
#include "../SymbolTable_checker/symbolTable.cpp"

symbolTable symbolTableInstant;
TokenType pendingType = TokenType::UNKNOWN;


class lexer {
private:
    std::string source;
    int pos;
    char current_char;
    int line_number;
    int char_position;

    void advance() {
        pos++;
        if (pos >= source.length()) {
            current_char = EOF;  // Indicates end of source
        } else {
            current_char = source[pos];
            char_position++;
        }
    }

    void skipWhitespace() {
        while (current_char != EOF && std::isspace(current_char)) {
            if (current_char == '\n' ) {
                line_number++;
                char_position = 1; // Start counting from 1 for the next line
            } else {
                char_position++;
            }
            advance();
        }
    }

    tokenizer getIdentifier() {
        int start_pos = pos;
        while (current_char != EOF && std::isalnum(current_char)) {
            advance();
        }
        std::string value = source.substr(start_pos, pos - start_pos);

        // Check if the identifier is a type or just an identifier

        TokenType type;
        if (value == "int") type = TokenType::INT;
        else if (value == "float") type = TokenType::FLOAT;
        else if (value == "char") type = TokenType::CHAR;
        else if (value == "bool") type = TokenType::BOOL;
        else if (value == "if") type = TokenType::IF;
        else if (value == "else") type = TokenType::ELSE;
        else if (value == "while") type = TokenType::WHILE;
        else type = TokenType::IDENTIFIER;
        if (type != TokenType::IDENTIFIER) {
            pendingType = type;
        } else if (pendingType != TokenType::UNKNOWN) {
            if (!symbolTableInstant.exists(value)) {
                symbolTableInstant.insert(value, pendingType);
            }
        }
        return tokenizer(type, value, line_number, char_position);
    }
    tokenizer getNumber() {
        int start_pos = pos;
        bool is_float = false;
        while (current_char != EOF && (std::isdigit(current_char) || current_char == '.')) {
            if (current_char == '.') {
                if (is_float) break;
                is_float = true;
            }
            advance();
        }

        std::string value = source.substr(start_pos, pos - start_pos);
        return tokenizer(TokenType::NUMBER, value, line_number, char_position);  // Can differentiate between INT_NUMBER and FLOAT_NUMBER if needed
    }
    tokenizer getChar() {
        advance();  // Consume the opening quote
        char value = current_char;
        advance();

        if (current_char == '\'') {
            advance();
            return tokenizer(TokenType::CHAR_VALUE, std::string(1, value), line_number, char_position);
        } else {
            // Handle character constant error or multiple characters between quotes
            return tokenizer(TokenType::EOF_TOK, "Invalid char constant", line_number, char_position);
        }
    }


public:
    lexer(const std::string& source)
            : source(source), pos(0), current_char(source[0]), line_number(1), char_position(0) {}

    tokenizer getNextToken() {
        while (current_char != EOF) {
            if (std::isspace(current_char)) {
                skipWhitespace();
                continue;
            }

            else if (std::isalpha(current_char)) {
                // Use your getIdentifier function to get the token for the identifier
                tokenizer token = getIdentifier();

                // If the token type indicates that it's a regular identifier, then check if it's in the symbol table.
                if (token.getType() == TokenType::IDENTIFIER) {
                    // Add to symbol table if not already present
                    if (!symbolTableInstant.exists(token.getValue())) {
                        symbolTableInstant.insert(token.getValue(), TokenType::IDENTIFIER);
                    }
                }

                return token;
            }

            else if (std::isdigit(current_char) || (current_char == '.' && std::isdigit(source[pos + 1]))) {
                return getNumber();
            }
            else {
                switch (current_char) {
                    case '+':
                        advance();
                        if (current_char == '+') {
                            advance();
                            return tokenizer(TokenType::INCREMENT, "++", line_number, char_position - 1); // Added token type INCREMENT
                        }
                        return tokenizer(TokenType::ADD_OP, "+", line_number, char_position);

                    case '-':
                        // Similar logic for decrement (--) can be added here, if necessary.
                        advance();
                        return tokenizer(TokenType::SUB_OP, "-", line_number, char_position);

                    case '*':
                        advance();
                        return tokenizer(TokenType::MUL_OP, "*", line_number, char_position);

                    case '/':
                        advance();
                        return tokenizer(TokenType::DIV_OP, "/", line_number, char_position);

                    case '%':
                        advance();
                        return tokenizer(TokenType::MOD_OP, "%", line_number, char_position);

                    case '&':
                        // Assuming && for logical AND.
                        advance();
                        if (current_char == '&') {
                            advance();
                            return tokenizer(TokenType::LOGICAL_AND, "&&", line_number, char_position - 1);
                        }
                        return tokenizer(TokenType::AND_OP, "&", line_number, char_position);

                    case '|':
                        // Assuming || for logical OR.
                        advance();
                        if (current_char == '|') {
                            advance();
                            return tokenizer(TokenType::LOGICAL_OR, "||", line_number, char_position - 1);
                        }
                        return tokenizer(TokenType::OR_OP, "|", line_number, char_position);

                    case '!':
                        advance();
                        if (current_char == '=') {
                            advance();
                            return tokenizer(TokenType::NOT_EQUAL, "!=", line_number, char_position - 1);
                        }
                        return tokenizer(TokenType::NOT_OP, "!", line_number, char_position);

                    case '=':
                        advance();
                        if (current_char == '=') {
                            advance();
                            return tokenizer(TokenType::EQUAL, "==", line_number, char_position - 1);
                        }
                        return tokenizer(TokenType::ASSIGNMENT, "=", line_number, char_position);

                    case '<':
                        advance();
                        if (current_char == '=') {
                            advance();
                            return tokenizer(TokenType::LESS_EQUAL, "<=", line_number, char_position - 1);
                        }
                        return tokenizer(TokenType::LESS_THAN, "<", line_number, char_position);

                    case '>':
                        advance();
                        if (current_char == '=') {
                            advance();
                            return tokenizer(TokenType::GREATER_EQUAL, ">=", line_number, char_position - 1);
                        }
                        return tokenizer(TokenType::GREATER_THAN, ">", line_number, char_position);

                    case '(':
                        advance();
                        return tokenizer(TokenType::L_PAREN, "(", line_number, char_position);

                    case ')':
                        advance();
                        return tokenizer(TokenType::R_PAREN, ")", line_number, char_position);

                    case '{':
                        advance();
                        return tokenizer(TokenType::L_BRACE, "{", line_number, char_position);

                    case '}':
                        advance();
                        return tokenizer(TokenType::R_BRACE, "}", line_number, char_position);

                    case ',':
                        advance();
                        return tokenizer(TokenType::COMMA, ",", line_number, char_position);

                    case ';':
                        advance();
                        pendingType = TokenType::UNKNOWN;  // Clear the pending type upon encountering a semicolon
                        return tokenizer(TokenType::SEMICOLON, ";", line_number, char_position);

                    default:
                        advance();
                        return tokenizer(TokenType::EOF_TOK, "Unrecognized token", line_number, char_position);
                }
            }

        }
        return tokenizer(TokenType::EOF_TOK, "", line_number, char_position);  // Return an EOF token if we've reached the end of the source
    }

    const symbolTable getSymbolTable() const {
        return symbolTableInstant;
    }
};