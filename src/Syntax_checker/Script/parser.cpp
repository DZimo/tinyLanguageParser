
#include "parser.h"

class parser {
private:
    lexer lexer_inst;
    overflower overflower;
    std::unique_ptr<astNode> sequence();
    std::string lastDataType;
    symbolTable symbolTable;
    bool insideFunctionScope = false;

protected:
    tokenizer current_token_inst;
public:
    explicit parser(const lexer& lex)
            : lexer_inst(lex),
              current_token_inst(lexer_inst.getNextToken()) {}// Only initialize here
    int evaluate(const std::unique_ptr<astNode>& node) {
        if (node->getType() == "Number") {
            NumberNode* numberNode = dynamic_cast<NumberNode*>(node.get());
            return std::stoi(numberNode->value);
        }
        else if (node->getType() == "BinaryExpression") {
            BinaryOpNode* binaryOpNode = dynamic_cast<BinaryOpNode*>(node.get());
            int leftVal = evaluate(binaryOpNode->left);
            int rightVal = evaluate(binaryOpNode->right);
            if (binaryOpNode->op == TokenType::ADD_OP) {
                if (overflower.additionWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("Addition will overflow");
                }
                return leftVal + rightVal;
            }
            // Handle other operations (SUB_OP, MUL_OP, etc.)
        }
        else if (node->getType() == "Variable") {
            VariableNode* variableNode = dynamic_cast<VariableNode*>(node.get());
            astNode* varNode = symbolTable.lookupSymbol(variableNode->name);
            if (varNode) {
                return evaluate(varNode);
            }
            else {
                throw std::runtime_error("Variable not found: " + variableNode->name);
            }
        }

        else if (node->getType() == "Declaration") {
            DeclarationNode* declarationNode = dynamic_cast<DeclarationNode*>(node.get());
            astNode* varNode = symbolTable.lookupSymbol(declarationNode->name);
            if (varNode) {
                return evaluate(varNode);
            }
            else {
                throw std::runtime_error("Variable not found: " + declarationNode->name);
            }
        }
        return 0; // Default return value, consider throwing an exception
    }

    int evaluate(astNode* node) {
        if (node->getType() == "Number") {
            NumberNode* numberNode = dynamic_cast<NumberNode*>(node);
            return std::stoi(numberNode->value);
        }
        else if (node->getType() == "BinaryExpression") {
            BinaryOpNode* binaryOpNode = dynamic_cast<BinaryOpNode*>(node);
            int leftVal = evaluate(binaryOpNode->left.get());
            int rightVal = evaluate(binaryOpNode->right.get());
            if (binaryOpNode->op == TokenType::ADD_OP) {
                if (overflower.additionWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("Addition will overflow");
                }
                return leftVal + rightVal;
            }
            // Handle other operations (SUB_OP, MUL_OP, etc.)
        }
        else if (node->getType() == "Variable") {
            VariableNode* variableNode = dynamic_cast<VariableNode*>(node);
            astNode* varNode = symbolTable.lookupSymbol(variableNode->name);
            if (varNode) {
                return evaluate(varNode);
            }
            else {
                throw std::runtime_error("Variable not found: " + variableNode->name);
            }
        }

        else if (node->getType() == "Declaration") {
            DeclarationNode* declarationNode = dynamic_cast<DeclarationNode*>(node);
            // Assuming DeclarationNode contains a child for its value
            return evaluate(declarationNode->value.get());
        }
        return 0; // Default return value, consider throwing an exception
    }

    void eat(TokenType token_type) {
        if (current_token_inst.type == token_type) {
            current_token_inst = lexer_inst.getNextToken();
        } else {
            throw std::runtime_error("Invalid Program : Expected " + tokenTypeToString(token_type));
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
        int leftValue, rightValue;

        while (current_token_inst.type == TokenType::ADD_OP || current_token_inst.type == TokenType::SUB_OP) {
            auto token = current_token_inst;
            eat(token.type);

            leftValue = this->evaluate(node.get());

            auto rightNode = term();
            rightValue = this->evaluate(rightNode.get());

            if (token.type == TokenType::ADD_OP) {
                if (overflower.additionWillOverflow(leftValue, rightValue)) {
                    throw std::overflow_error("The program contains an integer overflow at line " +
                                              std::to_string(lexer_inst.line_number) + ". The sum " +
                                              std::to_string(leftValue) + " + " +
                                              std::to_string(rightValue) +
                                              " overflows the maximum integer size (16 bit integer).");
                }
            } else if (token.type == TokenType::SUB_OP) {
                if (overflower.subtractionWillOverflow(leftValue, rightValue)) {
                    throw std::overflow_error("The program contains an integer underflow at line " +
                                              std::to_string(lexer_inst.line_number) + ". The subtraction " +
                                              std::to_string(leftValue) + " - " +
                                              std::to_string(rightValue) +
                                              " underflows the minimum integer size (16 bit integer).");
                }
            }

            node = std::make_unique<BinaryOpNode>(std::move(node), token.type, std::move(rightNode));
        }

        return node;
    }

    std::unique_ptr<astNode> term() {
        auto node = factor();

        while (current_token_inst.type == TokenType::MUL_OP || current_token_inst.type == TokenType::DIV_OP || current_token_inst.type == TokenType::MOD_OP) {
            auto token = current_token_inst;
            if (token.type == TokenType::MUL_OP) {
                //overflower.multiplicationWillOverflow();
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
        } else if (token.type == TokenType::CHAR_VALUE) {
            eat(TokenType::CHAR_VALUE);
            return std::make_unique<CharacterNode>(token.value[0]);
        }else if (token.type == TokenType::BOOL_VALUE) {
            eat(TokenType::BOOL_VALUE);
            bool boolValue = (token.value == "true") ? true : false;
            return std::make_unique<BooleanNode>(boolValue);
        }
        else if (token.type == TokenType::IDENTIFIER) {
            auto potentialVarName = token.value;
            eat(TokenType::IDENTIFIER);

            // Check if this variable exists in this scope
            if(!symbolTable.lookupSymbol(potentialVarName)) {
                throw std::runtime_error("Invalid Program : Use of undeclared variable " + potentialVarName);
            }
            if (current_token_inst.type == TokenType::L_PAREN) {
                return functionCall(potentialVarName);
            } else {
                return std::make_unique<VariableNode>(potentialVarName);
            }
        } else if (token.type == TokenType::L_PAREN) {
            eat(TokenType::L_PAREN);
            auto node = expr();
            eat(TokenType::R_PAREN);
            return node;
        } else {
            throw std::runtime_error("Invalid Program : Invalid token in factor!");
        }
    }

    std::vector<std::unique_ptr<astNode>> parseAST() {
        std::vector<std::unique_ptr<astNode>> nodes;
        symbolTable.pushScope();
        // Keep parsing until we hit the end of file token.
        while (current_token_inst.type != TokenType::EOF_TOK) {
            auto node = statement();
            // Only push back non-null nodes.
            if (node) {
                nodes.push_back(std::move(node));
            }

            // Now expect a semicolon or end of file after a non-null statement.
            if (node != nullptr && current_token_inst.type != TokenType::SEMICOLON && current_token_inst.type != TokenType::EOF_TOK) {
                throw std::runtime_error("Invalid Program : Expected end of statement!");
            }

            // If it was a semicolon, move to the next token.
            if (current_token_inst.type == TokenType::SEMICOLON) {
                eat(TokenType::SEMICOLON);
            }
        }
        return nodes;
    }

    std::unique_ptr<astNode> declaration() {
        // Check to disallow global variables declaration
        auto type = current_token_inst.type;
        lastDataType = tokenTypeToString(current_token_inst.type); // Store the data type

        eat(current_token_inst.type);  // Eat the type token.

        auto varName = current_token_inst.value;
        if(current_token_inst.type != TokenType::IDENTIFIER)
        {
            throw std::runtime_error("Invalid Program : Expected identifier after type");
        }
        if (lexer_inst.getNextToken().type == TokenType::ASSIGNMENT) {
            throw std::runtime_error("Invalid Program : Direct assignment during declaration is not allowed");
        }
        lexer_inst.retreat();
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
                    // Here we check if it is a function call or declaration
                    eat(TokenType::L_PAREN);
                    symbolTable.pushScope();
                    std::vector<std::pair<std::string, std::string>> parameters;
                    if (current_token_inst.type != TokenType::R_PAREN) {
                        do{  // This loop will handle multiple parameters
                            // Expecting a type (like INT, CHAR, etc.)
                            if (current_token_inst.type == TokenType::INT ||
                                current_token_inst.type == TokenType::CHAR ||
                                current_token_inst.type == TokenType::FLOAT ||
                                current_token_inst.type == TokenType::BOOL) {
                                auto currentType = type();
                                std::string paramType = currentType;
                                //eat(current_token_inst.type);

                                // Expecting an identifier for the parameter name
                                if (current_token_inst.type != TokenType::IDENTIFIER) {
                                    throw std::runtime_error("Invalid Program : Expected parameter name");
                                }
                                std::string paramName = current_token_inst.value;
                                eat(TokenType::IDENTIFIER);

                                // Add to parameters list
                                parameters.emplace_back(paramType, paramName);
                                auto declarationNode = std::make_unique<DeclarationNode>(lastDataType,paramName);
                                symbolTable.insertSymbol(paramName, declarationNode.get());
                                // If next token is a comma, then eat it and continue the loop for next parameter
                                if (current_token_inst.type == TokenType::COMMA) {
                                    eat(TokenType::COMMA);
                                } else if (current_token_inst.type == TokenType::R_PAREN) {
                                    // If it's a right parenthesis, then parameter list has ended
                                    break;
                                } else {
                                    // Anything other than a comma or right parenthesis is an error
                                    throw std::runtime_error("Invalid Program : Expected , or ) after parameter declaration");
                                }
                            }
                            else {
                                return functionCall(varOrFuncName);
                            }
                        } while (current_token_inst.type != TokenType::R_PAREN);
                    }
                    eat(TokenType::R_PAREN);
                    if (current_token_inst.type != TokenType::L_BRACE) {
                        throw std::runtime_error("Invalid Program : Expected { after function definition");
                    }

                    auto funcBody = blockStatement();
                    if (BlockNode* blockNode = dynamic_cast<BlockNode*>(funcBody.get())) {
                        auto functionNode = std::make_unique<FunctionDeclarationNode>(varOrFuncName, std::move(parameters), std::move(blockNode->statements));
                        //symbolTable.insertGlobalSymbol(varOrFuncName, functionNode.get());
                        symbolTable.insertSymbol(varOrFuncName,functionNode.get());
                        return functionNode;
                    } else {
                        throw std::runtime_error("Invalid Program : Expected a BlockNode for the function body.");
                    }
                }
                else if(current_token_inst.type != TokenType::L_PAREN)
                {
                    if(!insideFunctionScope)
                    {
                        throw std::runtime_error("Invalid Program : Global variables not allowed");
                    }
                    std::vector<std::unique_ptr<astNode>> variableNodes;
                    bool continueDeclaration = true;
                    while (continueDeclaration) {
                        // Check if it's an array declaration
                        int arraySize = -1;  // Default, meaning not an array
                        if(current_token_inst.type == TokenType::L_BRACKET) {
                            eat(TokenType::L_BRACKET);

                            if(current_token_inst.type != TokenType::NUMBER) {
                                throw std::runtime_error("Invalid Program : Array size should be a number");
                            }
                            arraySize = std::stoi(current_token_inst.value);
                            eat(TokenType::NUMBER);

                            if(current_token_inst.type != TokenType::R_BRACKET) {
                                throw std::runtime_error("Invalid Program : Expected closing bracket");
                            }
                            eat(TokenType::R_BRACKET);
                        }
                        if (current_token_inst.type == TokenType::ASSIGNMENT) {
                            // Check if this variable exists in this scope
                            if(!symbolTable.lookupSymbol(varOrFuncName)) {
                                throw std::runtime_error("Invalid Program : Use of undeclared variable " + varOrFuncName);
                            }
                            eat(TokenType::ASSIGNMENT);
                            // Handle the right-hand side. It can be an expression, a function call, another variable, or a literal.
                            auto rightExpr = expr();
                            auto rightExprCopy = deepCopyAstNode(rightExpr.get());

                            if(arraySize != -1) {
                                throw std::runtime_error("Invalid Program : Array initialization this way is not supported");
                            }
                            variableNodes.push_back(std::make_unique<AssignmentNode>(std::make_unique<IdentifierNode>(varOrFuncName), std::move(rightExpr)));
                            symbolTable.updateValueInScopes(varOrFuncName, std::move(rightExprCopy));
                        }
                        else {
                            // It's just a variable declaration without an assignment
                            if(arraySize != -1) {
                                variableNodes.push_back(std::make_unique<ArrayDeclarationNode>(lastDataType, varOrFuncName, arraySize));
                                symbolTable.insertSymbol(varOrFuncName, variableNodes.back().get());  // Assuming ArrayDeclarationNode derives from astNode

                            } else {
                                variableNodes.push_back(std::make_unique<DeclarationNode>(lastDataType, varOrFuncName));
                                symbolTable.insertSymbol(varOrFuncName, variableNodes.back().get());
                            }
                        }

                        if (current_token_inst.type == TokenType::COMMA) {
                            // Either normal assignment or function assignment
                            eat(TokenType::COMMA);
                            if (current_token_inst.type != TokenType::IDENTIFIER) {
                                throw std::runtime_error("Invalid Program : Expected another variable name after comma");
                            }
                            varOrFuncName = current_token_inst.value;
                            eat(TokenType::IDENTIFIER);
                        }
                        else if (current_token_inst.type == TokenType::SEMICOLON) {
                            // End of the declaration or assignment statement
                            eat(TokenType::SEMICOLON);
                            continueDeclaration = false;
                        }
                        else {
                            throw std::runtime_error("Invalid Program : Expected comma, assignment, or semicolon");
                        }
                    }

                    if (current_token_inst.type == TokenType::R_BRACE) { // Assuming R_CURLY represents the end of a block
                        //symbolTable.popScope();
                    }
                    // If you only have a single node, return it directly, otherwise return a compound node
                    if (variableNodes.size() == 1) {
                        return std::move(variableNodes[0]);
                    }
                    return std::make_unique<CompoundNode>(std::move(variableNodes));
                }
                else {
                    throw std::runtime_error("Invalid Program : Unexpected token after IDENTIFIER");
                }
            }
            case TokenType::IF:
                symbolTable.pushScope();
                return ifStatement();

            case TokenType::ELSE:
                symbolTable.pushScope();
                return elseStatement();

            case TokenType::WHILE:
                symbolTable.pushScope();
                return whileStatement();

            case TokenType::L_BRACE:
                symbolTable.pushScope();
                return blockStatement();

            case TokenType::RETURN:
                return returnStatement();

            case TokenType::SEMICOLON:
                // Empty statement.
                eat(TokenType::SEMICOLON);
                return {};

            default:
                throw std::runtime_error("Invalid Program : This token is not defined in Grammar");

        }
    }

    std::unique_ptr<astNode> deepCopyAstNode(const astNode* node) {
        if (node->getType() == "Number") {
            const NumberNode* numberNode = dynamic_cast<const NumberNode*>(node);
            return std::make_unique<NumberNode>(*numberNode);
        }
        else if (node->getType() == "BinaryExpression") {
            const BinaryOpNode* binaryOpNode = dynamic_cast<const BinaryOpNode*>(node);
            auto leftCopy = binaryOpNode->left ? deepCopyAstNode(binaryOpNode->left.get()) : nullptr;
            auto rightCopy = binaryOpNode->right ? deepCopyAstNode(binaryOpNode->right.get()) : nullptr;
            auto newNode = std::make_unique<BinaryOpNode>(std::move(leftCopy), binaryOpNode->op, std::move(rightCopy));
            return newNode;
        }
        // ... handle other node types ...

        return nullptr; // or throw an exception for an unsupported node type
    }

    std::unique_ptr<astNode> blockStatement() {
        // Allow local variables
        insideFunctionScope=true;
        eat(TokenType::L_BRACE);
        std::vector<std::unique_ptr<astNode>> statements;

        while (current_token_inst.type != TokenType::R_BRACE && current_token_inst.type != TokenType::EOF_TOK) {
            auto stmt = statement();
            if (stmt) {  // Check if stmt is non-null
                statements.push_back(std::move(stmt));
            }
        }
        eat(TokenType::R_BRACE);
        // Disallow local variables
        insideFunctionScope= false;
        symbolTable.popScope();
        return std::make_unique<BlockNode>(std::move(statements));
    }

    std::unique_ptr<astNode> returnStatement() {
        eat(TokenType::RETURN);
        auto returnValue = expr();
        if (current_token_inst.type != TokenType::SEMICOLON) {
            throw std::runtime_error("Invalid Program : Expected ; after return statement");
        }
        eat(TokenType::SEMICOLON);
        return std::make_unique<ReturnNode>(std::move(returnValue));
    }

    std::unique_ptr<astNode> ifStatement() {
        eat(TokenType::IF);
        eat(TokenType::L_PAREN);
        auto condition = expr();
        eat(TokenType::R_PAREN);
        auto trueBranch = statement();
        if (current_token_inst.type != TokenType::ELSE) {
            throw std::runtime_error("Invalid Program : Expected else branch !");
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
        insideFunctionScope = true;
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

    std::unique_ptr<astNode> functionCall(const std::string& funcName) {
        eat(TokenType::L_PAREN);
        std::vector<std::unique_ptr<astNode>> arguments;
        while (current_token_inst.type != TokenType::R_PAREN) {
            arguments.push_back(expr());
            if (current_token_inst.type == TokenType::COMMA) {
                eat(TokenType::COMMA);
            } else if (current_token_inst.type != TokenType::R_PAREN) {
                throw std::runtime_error("Invalid Program : Expected comma or right parenthesis in function call");
            }
        }
        eat(TokenType::R_PAREN);

        return std::make_unique<FunctionCallNode>(funcName, std::move(arguments));
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
                throw std::runtime_error("Invalid Program : Expected , or ) in parameter list");
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
                throw std::runtime_error("Invalid Program : Invalid type!");
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