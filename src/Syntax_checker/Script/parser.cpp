
// Created by Admin on 8/11/2023.
//

#include <memory>
#include "parser.h"

#include "../../Lexical_checker/lexer.cpp"
#include "../programNode.cpp"



class parser {
private:
    lexer lexer_inst;
    std::unique_ptr<astNode> sequence();
    std::string lastDataType;
protected:
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
            throw std::runtime_error("Expected " + tokenTypeToString(token_type));
        }
    }

    std::unique_ptr<astNode> expr() {
        auto node = additiveExpr();

        while (current_token_inst.type == TokenType::LESS_THAN || current_token_inst.type == TokenType::LESS_EQUAL ||
               current_token_inst.type == TokenType::GREATER_THAN || current_token_inst.type == TokenType::GREATER_EQUAL ||
               current_token_inst.type == TokenType::EQUAL || current_token_inst.type == TokenType::NOT_EQUAL) {

            auto token = current_token_inst;
            eat(token.type);  // Consume the operator

            node = std::make_unique<BinaryOpNode>(std::move(node), token.type, additiveExpr());
        }

        return node;
    }

    std::unique_ptr<astNode> additiveExpr() {
        auto node = term();

        while (current_token_inst.type == TokenType::ADD_OP || current_token_inst.type == TokenType::SUB_OP) {
            auto token = current_token_inst;
            eat(token.type);

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
                return std::make_unique<AssignmentNode>(std::move(node), expr());
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
            if (node) {
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
        lastDataType = tokenTypeToString(current_token_inst.type); // Store the data type

        eat(current_token_inst.type);  // Eat the type token.

        auto varName = current_token_inst.value;
        if(current_token_inst.type != TokenType::IDENTIFIER)
        {
            throw std::runtime_error("Expected identifier after type");
        }
        return statement();
        /*
        if (current_token_inst.type == TokenType::L_PAREN) {
            // Go back a step and re-parse the type and function name in the function() method.
            // This is a simple way to handle this, but there may be more efficient approaches.
            //current_token_inst.type = type;
            //current_token_inst.value = varName;

            // Handle function declaration/definition.
            //return function();
            return std::make_unique<DeclarationNode>(tokenTypeToString(type), varName);

        }
        else if (current_token_inst.type == TokenType::ASSIGNMENT) {
            eat(TokenType::ASSIGNMENT);
            auto initializer = expr();
            eat(TokenType::SEMICOLON);

            // Assuming the assignment node can represent an initialization.
            return std::make_unique<AssignmentNode>(std::make_unique<IdentifierNode>(varName), std::move(initializer));
        }
        else {
            eat(TokenType::SEMICOLON);
            // Use your original DeclarationNode for simple declarations without initialization.
            return std::make_unique<DeclarationNode>(tokenTypeToString(type), varName);
        }*/
    }

    std::unique_ptr<astNode> statement() {
        switch (current_token_inst.type) {
            case TokenType::INT:
            case TokenType::CHAR:
            case TokenType::FLOAT:
            case TokenType::BOOL:
                return declaration();

            case TokenType::IDENTIFIER: {
                auto varOrFuncName = current_token_inst.value;
                eat(TokenType::IDENTIFIER);
                if (current_token_inst.type == TokenType::L_PAREN) {
                    eat(TokenType::L_PAREN);
                    // Here, you would parse the parameters into 'parameters' vector.
                    std::vector<std::pair<std::string, std::string>> parameters;
                    if (current_token_inst.type != TokenType::R_PAREN) {
                        do{  // This loop will handle multiple parameters
                            // Expecting a type (like INT, CHAR, etc.)
                            auto currentType = type();
                            std::string paramType = currentType;
                            //eat(current_token_inst.type);

                            // Expecting an identifier for the parameter name
                            if (current_token_inst.type != TokenType::IDENTIFIER) {
                                throw std::runtime_error("Expected parameter name");
                            }
                            std::string paramName = current_token_inst.value;
                            eat(TokenType::IDENTIFIER);

                            // Add to parameters list
                            parameters.emplace_back(paramType, paramName);

                            // If next token is a comma, then eat it and continue the loop for next parameter
                            if (current_token_inst.type == TokenType::COMMA) {
                                eat(TokenType::COMMA);
                            } else if (current_token_inst.type == TokenType::R_PAREN) {
                                // If it's a right parenthesis, then parameter list has ended
                                break;
                            } else {
                                // Anything other than a comma or right parenthesis is an error
                                throw std::runtime_error("Expected , or ) after parameter declaration");
                            }
                        } while (current_token_inst.type != TokenType::R_PAREN);
                    }
                    eat(TokenType::R_PAREN);

                    if (current_token_inst.type != TokenType::L_BRACE) {
                        throw std::runtime_error("Expected { after function definition");
                    }

                    auto funcBody = blockStatement();
                    if (BlockNode* blockNode = dynamic_cast<BlockNode*>(funcBody.get())) {
                        return std::make_unique<FunctionDeclarationNode>(varOrFuncName,
                                                                         std::move(parameters),
                                                                         std::move(blockNode->statements));
                    } else {
                        throw std::runtime_error("Expected a BlockNode for the function body.");
                    }
                }
                else if (current_token_inst.type == TokenType::ASSIGNMENT) {
                    // Handle variable assignment

                    eat(TokenType::ASSIGNMENT);
                    auto rightExpr = expr();
                    return std::make_unique<AssignmentNode>(std::make_unique<IdentifierNode>(varOrFuncName), std::move(rightExpr));
                }
                else if (current_token_inst.type == TokenType::SEMICOLON) {
                    // It's a simple variable declaration
                    eat(TokenType::SEMICOLON);
                    // Assuming you have a variable `lastDataType` that stores the last parsed data type.
                    // It can be set in the declaration() function after you consume a type.
                    return std::make_unique<DeclarationNode>(lastDataType, varOrFuncName);
                }
                else if (current_token_inst.type == TokenType::COMMA) {

                }
                else {
                    throw std::runtime_error("Unexpected token after IDENTIFIER");
                }
            }

            case TokenType::IF:
                return ifStatement();

            case TokenType::ELSE:
                return elseStatement();

            case TokenType::WHILE:
                return whileStatement();

            case TokenType::L_BRACE:
                return blockStatement();

            case TokenType::RETURN:
                return returnStatement();

            case TokenType::SEMICOLON:
                // Empty statement.
                eat(TokenType::SEMICOLON);
                return {};

            default:
                throw std::runtime_error("This token is not defined in Grammar");
        }
    }

    std::unique_ptr<astNode> blockStatement() {
        eat(TokenType::L_BRACE);
        std::vector<std::unique_ptr<astNode>> statements;

        while (current_token_inst.type != TokenType::R_BRACE && current_token_inst.type != TokenType::EOF_TOK) {
            auto stmt = statement();
            if (stmt) {  // Check if stmt is non-null
                statements.push_back(std::move(stmt));
            }
        }

        eat(TokenType::R_BRACE);

        return std::make_unique<BlockNode>(std::move(statements));
    }

    std::unique_ptr<astNode> returnStatement() {
        eat(TokenType::RETURN);

        // Parse the expression after the 'return' keyword
        auto returnValue = expr();

        if (current_token_inst.type != TokenType::SEMICOLON) {
            throw std::runtime_error("Expected ; after return statement");
        }
        eat(TokenType::SEMICOLON);

        // Assuming you have a ReturnNode defined in your AST like this:
        // class ReturnNode : public astNode { ... }
        // The ReturnNode constructor might take the returned expression as an argument.
        return std::make_unique<ReturnNode>(std::move(returnValue));
    }

    std::unique_ptr<astNode> ifStatement() {
        eat(TokenType::IF);
        eat(TokenType::L_PAREN);
        auto condition = expr();
        eat(TokenType::R_PAREN);
        auto trueBranch = statement();
        if (current_token_inst.type != TokenType::ELSE) {
            throw std::runtime_error("Expected else branch !");
        }
        eat(TokenType::ELSE);
        std::unique_ptr<astNode> falseBranch = statement();
        return std::make_unique<IfNode>(std::move(condition), std::move(trueBranch), std::move(falseBranch));
    }

    std::unique_ptr<astNode> elseStatement() {
        eat(TokenType::ELSE);
        return statement();
    }

    std::unique_ptr<astNode> whileStatement() {
        eat(TokenType::WHILE);
        eat(TokenType::L_PAREN);
        auto condition = expr();
        eat(TokenType::R_PAREN);
        auto loopBody = statement();

        return std::make_unique<WhileNode>(std::move(condition), std::move(loopBody));
    }

    std::unique_ptr<astNode> main_function() {
        eat(TokenType::INT);
        eat(TokenType::MAIN);
        eat(TokenType::L_PAREN);
        eat(TokenType::R_PAREN);
        eat(TokenType::L_BRACE);

        std::vector<std::unique_ptr<astNode>> decls;
        while (current_token_inst.type == TokenType::INT ||
               current_token_inst.type == TokenType::FLOAT ||
               current_token_inst.type == TokenType::CHAR ||
               current_token_inst.type == TokenType::BOOL) {
            decls.push_back(declaration());
        }

        std::vector<std::unique_ptr<astNode>> stmts;
        while (current_token_inst.type != TokenType::R_BRACE) {
            stmts.push_back(statement());
        }

        eat(TokenType::R_BRACE);

        return std::make_unique<MainFunctionNode>(std::move(decls), std::move(stmts));
    }

    std::unique_ptr<astNode> functionCall() {
        auto funcName = current_token_inst.value;
        eat(TokenType::IDENTIFIER);
        eat(TokenType::L_PAREN);

        std::vector<std::unique_ptr<astNode>> args;
        while (current_token_inst.type != TokenType::R_PAREN) {
            args.push_back(expr());
            if (current_token_inst.type == TokenType::COMMA) {
                eat(TokenType::COMMA);
            }
        }
        eat(TokenType::R_PAREN);

        return std::make_unique<FunctionCallNode>(funcName, std::move(args));
    }

    std::unique_ptr<astNode> function() {
        // 1. Parse the return type of the function
        auto returnType = type();

        // 2. Parse the function name
        auto funcName = current_token_inst.value;
        eat(TokenType::IDENTIFIER);

        // 3. Parse the parameter list
        eat(TokenType::L_PAREN);
        std::vector<std::pair<std::string, std::string>> parameters;

        // Check if there are parameters in the list
        while (current_token_inst.type != TokenType::R_PAREN) {
            // Parse the type of the parameter
            auto paramType = type();
            // Parse the name of the parameter
            auto paramName = current_token_inst.value;
            parameters.push_back({paramType, paramName});
            eat(TokenType::IDENTIFIER);

            // If there's a comma, then we expect another parameter
            if (current_token_inst.type == TokenType::COMMA) {
                eat(TokenType::COMMA);
            } else if (current_token_inst.type != TokenType::R_PAREN) {
                // If it's not a comma and not a closing parenthesis, it's an error
                throw std::runtime_error("Expected , or ) in parameter list");
            }
        }
        eat(TokenType::R_PAREN);

        // 4. Parse the function body

        // Handle declarations inside the function
        std::vector<std::unique_ptr<astNode>> decls;
        while (current_token_inst.type == TokenType::INT ||
               current_token_inst.type == TokenType::FLOAT ||
               current_token_inst.type == TokenType::CHAR ||
               current_token_inst.type == TokenType::BOOL) {
            decls.push_back(declaration());
        }

        // Handle statements inside the function
        eat(TokenType::L_BRACE);
        std::vector<std::unique_ptr<astNode>> stmts;
        while (current_token_inst.type != TokenType::R_BRACE) {
            stmts.push_back(statement());
        }
        eat(TokenType::R_BRACE);

        return std::make_unique<FunctionNode>(returnType, funcName, std::move(parameters), std::move(decls), std::move(stmts));
    }

    std::unique_ptr<astNode> tiny_program() {
        auto mainFuncNode = main_function();

        // Create a program node with an empty vector of statements
        std::unique_ptr<ProgramNode> program = std::make_unique<ProgramNode>(std::vector<std::unique_ptr<astNode>>{});

        // Add the main function to the functions vector
        program->addFunction(std::move(mainFuncNode));

        // If there are more functions after the main function, parse them
        while (current_token_inst.type != TokenType::EOF_TOK &&
               (current_token_inst.type == TokenType::INT ||
                current_token_inst.type == TokenType::FLOAT ||
                current_token_inst.type == TokenType::CHAR ||
                current_token_inst.type == TokenType::BOOL)) {
            auto funcNode = function();
            program->addFunction(std::move(funcNode));
        }

        return program;
    }

    std::string type() {
        switch (current_token_inst.type) {
            case TokenType::INT:
                eat(TokenType::INT);
                return "int";
            case TokenType::FLOAT:
                eat(TokenType::FLOAT);
                return "float";
            case TokenType::CHAR:
                eat(TokenType::CHAR);
                return "char";
            case TokenType::BOOL:
                eat(TokenType::BOOL);
                return "bool";
            default:
                throw std::runtime_error("Invalid type!");
        }
    }

    std::string serializeAST(const std::vector<std::unique_ptr<astNode>>& astNodes) {
        std::string result = "[";
        for (size_t i = 0; i < astNodes.size(); ++i) {
            if (astNodes[i]) {  // Check if node is not nullptr
                result += astNodes[i]->toJSON();
                if (i != astNodes.size() - 1) {
                    result += ", ";
                }
            }
        }
        result += "]";
        return result;
    }
};