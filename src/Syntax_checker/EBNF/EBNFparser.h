/*
#ifndef TINYLANGUAGEPARSER_EBNFPARSER_H
#define TINYLANGUAGEPARSER_EBNFPARSER_H

#include "../../Lexical_checker/EBNFlexer.h"
#include "../../AST_checker/astNode.h"
#include <vector>
#pragma once

class EBNFParser {
private:
    EBNFlexer lexer_instance;
    EBNFtokenizer current_token_instance;

    void eat(EBNFtokenType token_type) {
        if (current_token_instance.type == token_type) {
            current_token_instance = lexer_instance.getNextToken();
        } else {
            throw std::runtime_error("Invalid Program : Token type mismatch!");
        }
    }
public:
    EBNFParser(EBNFlexer& lex) : lexer_instance(lex), current_token_instance(lexer_instance.getNextToken()) {}

    std::string identifier() {
        if(current_token_instance.type != EBNFtokenType::IDENTIFIER) {
            throw std::runtime_error("Invalid Program : Expected identifier");
        }
        std::string idValue = current_token_instance.value;
        eat(EBNFtokenType::IDENTIFIER);
        return idValue;
    }

    std::unique_ptr<astNode> factor() {
        if(current_token_instance.type == EBNFtokenType::IDENTIFIER) {
            std::string idValue = identifier();
            return std::make_unique<IdentifierNode>(idValue);
        } else if(current_token_instance.type == EBNFtokenType::L_PAREN) {
            eat(EBNFtokenType::L_PAREN);
            auto expr = expression();
            eat(EBNFtokenType::R_PAREN);
            return expr;
        } else {
            throw std::runtime_error("Invalid factor in EBNF expression");
        }
    }

    std::unique_ptr<astNode> term() {
        auto left = factor();
        while(current_token_instance.type != EBNFtokenType::EOF_TOK &&
                current_token_instance.type != EBNFtokenType::R_PAREN &&
                current_token_instance.type != EBNFtokenType::OR_OP) {
            auto right = factor();
            left = std::make_unique<ConcatNode>(std::move(left), std::move(right));
        }
        return left;
    }

    std::unique_ptr<astNode> expression() {
        auto left = term();
        while(current_token_instance.type == EBNFtokenType::OR_OP) {
            eat(EBNFtokenType::OR_OP);
            auto right = term();
            left = std::make_unique<AlternativeNode>(std::move(left), std::move(right));
        }
        return left;
    }

    std::unique_ptr<astNode> rule() {
        auto id = identifier(); // This should return a std::string.
        if(current_token_instance.type != EBNFtokenType::ASSIGNMENT) {
            throw std::runtime_error("Invalid Program : Expected '=' after identifier in rule definition");
        }
        eat(EBNFtokenType::ASSIGNMENT);
        auto expr = expression(); // This should return a std::unique_ptr<astNode>.
        if(current_token_instance.type != EBNFtokenType::SEMICOLON) {
            throw std::runtime_error("Invalid Program : Expected ';' at the end of rule definition");
        }
        eat(EBNFtokenType::SEMICOLON);
        return std::make_unique<RuleNode>(id, std::move(expr));
    }

    std::vector<std::unique_ptr<astNode>> parseEBNF() {
        std::vector<std::unique_ptr<astNode>> astNodes;
        while(current_token_instance.type != EBNFtokenType::EOF_TOK) {
            astNodes.push_back(rule());
        }
        return astNodes;
    }
};

std::vector<std::unique_ptr<astNode>> parseWithGeneratedParser(const std::string& ebnfScript) {
    EBNFlexer lex(ebnfScript);
    EBNFParser ebnfParser(lex);
    return ebnfParser.parseEBNF();
}


#endif //TINYLANGUAGEPARSER_EBNFPARSER_H
*/