//
// Created by Admin on 8/11/2023.
//

#include <memory>
#include "parser.h"

#include "../AST_checker/assignementNode.cpp"
#include "../Lexical_checker/lexer.cpp"
#include "programNode.cpp"



class parser {
private:
    lexer lexer_inst;
    tokenizer current_token_inst;

public:
    explicit parser(const lexer& lex)
            : lexer_inst(lex),
              current_token_inst(lexer_inst.getNextToken()) // Only initialize here
    {}
    void eat(TokenType token_type) {
        if (current_token_inst.type == token_type) {
            current_token_inst = lexer_inst.getNextToken();
        } else {
            throw std::runtime_error("Token type mismatch!");
        }
    }

    std::unique_ptr<astNode> expr() {
        auto node = term();

        while (current_token_inst.type == TokenType::ADD_OP || current_token_inst.type == TokenType::SUB_OP) {
            auto token = current_token_inst;
            if (token.type == TokenType::ADD_OP) {
                eat(TokenType::ADD_OP);
            } else if (token.type == TokenType::SUB_OP) {
                eat(TokenType::SUB_OP);
            }

            node = std::make_unique<BinaryOpNode>(std::move(node), token.type, term());
        }

        return node;
    }

    std::unique_ptr<astNode> term() {
        auto node = factor();

        while (current_token_inst.type == TokenType::MUL_OP || current_token_inst.type == TokenType::DIV_OP || current_token_inst.type == TokenType::MOD_OP) {
            auto token = current_token_inst;
            if (token.type == TokenType::MUL_OP) {
                eat(TokenType::MUL_OP);
            } else if (token.type == TokenType::DIV_OP) {
                eat(TokenType::DIV_OP);
            } else if (token.type == TokenType::MOD_OP) {
                eat(TokenType::MOD_OP);
            }

            node = std::make_unique<BinaryOpNode>(std::move(node), token.type, factor());
        }

        return node;
    }

    std::unique_ptr<astNode> factor() {
        auto token = current_token_inst;
        if (token.type == TokenType::NUMBER) {
            eat(TokenType::NUMBER);
            return std::make_unique<NumberNode>(token.value);
        } else if (token.type == TokenType::IDENTIFIER) {
            auto node = std::make_unique<VariableNode>(token.value);
            eat(TokenType::IDENTIFIER);

            // If an assignment operation follows an identifier, handle assignment
            if (current_token_inst.type == TokenType::ASSIGNMENT) {
                eat(TokenType::ASSIGNMENT);
                return std::make_unique<assignementNode>(std::move(node), expr());
            }

            return node;
        } else if (token.type == TokenType::L_PAREN) {
            eat(TokenType::L_PAREN);
            auto node = expr();
            eat(TokenType::R_PAREN);
            return node;
        } else {
            throw std::runtime_error("Invalid token in factor!");
        }
    }

    std::vector<std::unique_ptr<astNode>> parseAST() {
        std::vector<std::unique_ptr<astNode>> nodes;

        // Keep parsing until we hit the end of file token.
        while (current_token_inst.type != TokenType::EOF_TOK) {
            auto node = statement();

            // Only push back non-null nodes.
            if (node != nullptr) {
                nodes.push_back(std::move(node));
            }

            // Now expect a semicolon or end of file after a non-null statement.
            if (node != nullptr && current_token_inst.type != TokenType::SEMICOLON && current_token_inst.type != TokenType::EOF_TOK) {
                throw std::runtime_error("Expected end of statement!");
            }

            // If it was a semicolon, move to the next token.
            if (current_token_inst.type == TokenType::SEMICOLON) {
                eat(TokenType::SEMICOLON);
            }
        }

        return nodes;
    }

    std::unique_ptr<astNode> declaration() {
        auto type = current_token_inst.type;
        eat(current_token_inst.type);  // Eat the type token.

        auto varName = current_token_inst.value;
        eat(TokenType::IDENTIFIER);

        if (current_token_inst.type == TokenType::ASSIGNMENT) {
            eat(TokenType::ASSIGNMENT);
            auto initializer = expr();
            eat(TokenType::SEMICOLON);

            // Assuming the assignment node can represent an initialization.
            return std::make_unique<assignementNode>(std::make_unique<identifierNode>(varName), std::move(initializer));
        }

        eat(TokenType::SEMICOLON);

        // Here, we use your original DeclarationNode for simple declarations without initialization.
        return std::make_unique<DeclarationNode>(tokenTypeToString(type), varName);
    }

    std::unique_ptr<astNode> statement() {
        switch (current_token_inst.type) {
            case TokenType::INT:
            case TokenType::CHAR:
            case TokenType::FLOAT:
            case TokenType::BOOL:
                return declaration();



            case TokenType::IDENTIFIER: {
                auto funcName = current_token_inst.value;
                eat(TokenType::IDENTIFIER);

                if (current_token_inst.type == TokenType::L_PAREN) {
                    eat(TokenType::L_PAREN);
                    if (current_token_inst.type != TokenType::R_PAREN) {
                        // Handle parameters if your language supports it.
                        // Otherwise, throw an error if anything other than a R_PAREN is found.
                        throw std::runtime_error("Expected ) after function name");
                    }
                    eat(TokenType::R_PAREN);

                    if (current_token_inst.type != TokenType::L_BRACE) {
                        throw std::runtime_error("Expected { after function definition");
                    }

                    auto funcBody = blockStatement(); // Parse the function body
                    if (BlockNode* blockNode = dynamic_cast<BlockNode*>(funcBody.get())) {
                        return std::make_unique<FunctionDeclarationNode>(funcName,
                                                                         std::vector<std::pair<std::string, std::string>>(), // You need to add the parameter parsing
                                                                         std::move(blockNode->statements));
                    }
                    else {
                        throw std::runtime_error("Expected a BlockNode for the function body.");
                    }              }
                else if (current_token_inst.type == TokenType::ASSIGNMENT) {
                    eat(TokenType::ASSIGNMENT);
                    auto rightExpr = expr();
                    return std::make_unique<assignementNode>(std::make_unique<identifierNode>(funcName), std::move(rightExpr));
                } else {
                    throw std::runtime_error("Unexpected token after IDENTIFIER");
                }
            }
            case TokenType::IF:
                return ifStatement();

            case TokenType::WHILE:
                return whileStatement();

            case TokenType::L_BRACE:
                return blockStatement();

            case TokenType::RETURN:
                //return returnStatement();

            case TokenType::SEMICOLON:
                // Empty statement.
                eat(TokenType::SEMICOLON);

            case TokenType::EOF_TOK:
                // Handle end of file token, which means the parsing process should terminate.
                // Depending on your design, you might want to just return a special AST node or throw an exception.
                return nullptr; // or you could throw an exception or handle it in a way that makes sense for your parser.

            default:
                throw std::runtime_error("Unexpected token type");
        }
    }

    std::unique_ptr<astNode> blockStatement() {
        eat(TokenType::L_BRACE);
        std::vector<std::unique_ptr<astNode>> statements;

        while (current_token_inst.type != TokenType::R_BRACE && current_token_inst.type != TokenType::EOF_TOK) {
            statements.push_back(statement());
        }

        eat(TokenType::R_BRACE);

        return std::make_unique<BlockNode>(std::move(statements));
    }

    std::unique_ptr<astNode> ifStatement() {
        eat(TokenType::IF);
        eat(TokenType::L_PAREN);
        auto condition = expr();
        eat(TokenType::R_PAREN);

        auto trueBranch = statement();

        std::unique_ptr<astNode> falseBranch = nullptr;
        if (current_token_inst.type == TokenType::ELSE) {
            eat(TokenType::ELSE);
            falseBranch = statement();
        }

        return std::make_unique<IfNode>(std::move(condition), std::move(trueBranch), std::move(falseBranch));
    }

    std::unique_ptr<astNode> whileStatement() {
        eat(TokenType::WHILE);
        eat(TokenType::L_PAREN);
        auto condition = expr();
        eat(TokenType::R_PAREN);

        auto loopBody = statement();

        return std::make_unique<WhileNode>(std::move(condition), std::move(loopBody));
    }

    std::string serializeAST(const std::vector<std::unique_ptr<astNode>>& astNodes) {
        std::string result = "[";
        for (size_t i = 0; i < astNodes.size(); ++i) {
            result += astNodes[i]->toJSON();
            if (i != astNodes.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }
};