#include <unordered_set>
#include "parser.h"

class parser {
private:
    lexer lexer_inst;
    overflower overflowerInstance;
    std::unique_ptr<astNode> sequence();
    std::string lastDataType;
    symbolTable symbolTableInstance;
    bool insideFunctionScope = false;
    bool insideDeclarationScope = true;

protected:
    tokenizer current_token_inst;
    std::unordered_set<std::string> visitedSymbols;

public:
    explicit parser(const lexer& lex)
            : lexer_inst(lex),
              current_token_inst(lexer_inst.getNextToken()) {}
    /*int evaluate(const std::unique_ptr<astNode>& node) {
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
            else if (binaryOpNode->op == TokenType::SUB_OP) {
                if (overflower.subtractionWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("Subtraction will overflow");
                }
                return leftVal - rightVal;
            }
            else if (binaryOpNode->op == TokenType::MUL_OP) {
                if (overflower.multiplicationWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("Multiplication will overflow");
                }
                return leftVal * rightVal;
            }
            else if (binaryOpNode->op == TokenType::DIV_OP) {
                if (rightVal == 0) {
                    throw std::runtime_error("Division by zero");
                }
                return leftVal / rightVal;
            }
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
            else if (binaryOpNode->op == TokenType::SUB_OP) {
                if (overflower.subtractionWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("Subtraction will overflow");
                }
                return leftVal - rightVal;
            }
            else if (binaryOpNode->op == TokenType::MUL_OP) {
                if (overflower.multiplicationWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("Multiplication will overflow");
                }
                return leftVal * rightVal;
            }
            else if (binaryOpNode->op == TokenType::DIV_OP) {
                if (rightVal == 0) {
                    throw std::runtime_error("Division by zero");
                }
                return leftVal / rightVal; // Assuming integer division
            }
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
    }*/
    int evaluate(astNode* node) {
        if (!node) {
            throw std::runtime_error("Null node encountered");
        }

        std::string nodeType = node->getType();

        if (nodeType == "Number") {
            NumberNode* numberNode = dynamic_cast<NumberNode*>(node);
            return std::stoi(numberNode->value);
        }
        else if (nodeType == "BinaryExpression") {
            BinaryOpNode* binaryOpNode = dynamic_cast<BinaryOpNode*>(node);
            int leftVal = evaluate(binaryOpNode->left.get());
            int rightVal = evaluate(binaryOpNode->right.get());

            if (binaryOpNode->op == TokenType::MUL_OP) {
                if (overflowerInstance.multiplicationWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("The program contains an integer overflow at line " +
                                              std::to_string(lexer_inst.line_number) + ". The multiplication " +
                                              std::to_string(leftVal) + " * " +
                                              std::to_string(rightVal) +
                                              " overflows the maximum integer size (16 bit integer).");
                }
                return leftVal * rightVal;
            }
            else if (binaryOpNode->op == TokenType::ADD_OP) {
                if (overflowerInstance.additionWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("The program contains an integer overflow at line " +
                                              std::to_string(lexer_inst.line_number) + ". The addition " +
                                              std::to_string(leftVal) + " + " +
                                              std::to_string(rightVal) +
                                              " overflows the maximum integer size (16 bit integer).");
                }
                return leftVal + rightVal;
            }
            else if (binaryOpNode->op == TokenType::SUB_OP) {
                if (overflowerInstance.subtractionWillOverflow(leftVal, rightVal)) {
                    throw std::overflow_error("The program contains an integer overflow at line " +
                                              std::to_string(lexer_inst.line_number) + ". The subtraction " +
                                              std::to_string(leftVal) + " - " +
                                              std::to_string(rightVal) +
                                              " overflows the maximum integer size (16 bit integer).");
                }
                return leftVal - rightVal;
            }
            else if (binaryOpNode->op == TokenType::DIV_OP) {
                if (rightVal == 0) {
                    throw std::overflow_error("The program contains an integer overflow at line " +
                                              std::to_string(lexer_inst.line_number) + ". The division " +
                                              std::to_string(leftVal) + " / " +
                                              std::to_string(rightVal) +
                                              " overflows the maximum integer size (16 bit integer).");
                }
                return leftVal / rightVal;
            }
            else if (binaryOpNode->op == TokenType::MOD_OP) {
                if (rightVal == 0) {
                    throw std::overflow_error("The program contains an integer overflow at line " +
                                              std::to_string(lexer_inst.line_number) + ". The mudolo " +
                                              std::to_string(leftVal) + " % " +
                                              std::to_string(rightVal) +
                                              " overflows the maximum integer size (16 bit integer).");
                }
                return leftVal % rightVal;
            }
        }
        else if (nodeType == "Variable") {
            VariableNode* variableNode = dynamic_cast<VariableNode*>(node);
            astNode* varNode = symbolTableInstance.lookupSymbol(variableNode->name);
            if (varNode) {
                return evaluate(varNode);
            }
            else {
                throw std::runtime_error("Variable not found: " + variableNode->name);
            }
        }
        else if (nodeType == "Declaration") {
            DeclarationNode* declarationNode = dynamic_cast<DeclarationNode*>(node);
            return evaluate(declarationNode->value.get());
        }


        return 0;
    }

    void eat(TokenType token_type) {
        if (current_token_inst.type == token_type) {
            current_token_inst = lexer_inst.getNextToken();
        } else {
            throw std::runtime_error("Invalid Program : Expected " + tokenTypeToString(token_type)+ " at line " +
            std::to_string(lexer_inst.line_number));
        }
    }

    std::unique_ptr<astNode> expr() {
        auto node = additiveExpr();

        while (current_token_inst.type == TokenType::LESS_THAN || current_token_inst.type == TokenType::LESS_EQUAL ||
               current_token_inst.type == TokenType::GREATER_THAN || current_token_inst.type == TokenType::GREATER_EQUAL ||
               current_token_inst.type == TokenType::EQUAL || current_token_inst.type == TokenType::NOT_EQUAL) {

            auto token = current_token_inst;
            eat(token.type);

            node = std::make_unique<BinaryOpNode>(std::move(node), token.type, additiveExpr(), lexer_inst.line_number);
        }

        return node;
    }

    std::unique_ptr<astNode> additiveExpr() {
        auto node = term();
        int leftValue = 0, rightValue = 0;

        while (current_token_inst.type == TokenType::ADD_OP || current_token_inst.type == TokenType::SUB_OP) {
            auto token = current_token_inst;
            eat(token.type);

            leftValue = this->evaluate(node.get());

            auto rightNode = term();
            rightValue = this->evaluate(rightNode.get());

            if (token.type == TokenType::ADD_OP) {
//                if (overflowerInstance.additionWillOverflow(leftValue, rightValue)) {
//                    throw std::overflow_error("The program contains an integer overflow at line " +
//                                              std::to_string(lexer_inst.line_number) + ". The sum " +
//                                              std::to_string(leftValue) + " + " +
//                                              std::to_string(rightValue) +
//                                              " overflows the maximum integer size (16 bit integer).");
//                }
            }  else if (token.type == TokenType::SUB_OP) {
//                if (overflowerInstance.subtractionWillOverflow(leftValue, rightValue)) {
//                    throw std::overflow_error("The program contains an integer underflow at line " +
//                                              std::to_string(lexer_inst.line_number) + ". The subtraction " +
//                                              std::to_string(leftValue) + " - " +
//                                              std::to_string(rightValue) +
//                                              " underflows the minimum integer size (16 bit integer).");
//                }
            }
            node = std::make_unique<BinaryOpNode>(std::move(node), token.type, std::move(rightNode), lexer_inst.line_number);
        }

        return node;
    }

    std::unique_ptr<astNode> term() {
        auto node = factor();
        int leftValue = 0, rightValue = 0;

        while (current_token_inst.type == TokenType::MUL_OP || current_token_inst.type == TokenType::DIV_OP || current_token_inst.type == TokenType::MOD_OP) {
            auto token = current_token_inst;
//            auto lexerState = lexer_inst.saveState();

//            // Evaluate the nodes without consuming tokens
//            leftValue = this->evaluate(node.get());
//            eat(token.type);
//
//            auto rightNode = factor(); // Now get the right node
//            rightValue = this->evaluate(rightNode.get());


            if (token.type == TokenType::MUL_OP) {
                //overflower.multiplicationWillOverflow();
                if (overflowerInstance.multiplicationWillOverflow(leftValue, rightValue)) {
                        throw std::overflow_error("The program contains an integer underflow at line " +
                                                  std::to_string(lexer_inst.line_number) + ". The multiplication " +
                                                  std::to_string(leftValue) + " - " +
                                                  std::to_string(rightValue) +
                                                  " overflows the maximum integer size (16 bit integer).");
                }
                eat(TokenType::MUL_OP);
            } else if (token.type == TokenType::DIV_OP) {
                eat(TokenType::DIV_OP);
            } else if (token.type == TokenType::MOD_OP) {
                eat(TokenType::MOD_OP);
            }

//            lexer_inst.restoreState(lexerState);

            node = std::make_unique<BinaryOpNode>(std::move(node), token.type, factor(),lexer_inst.line_number);
            //node = std::make_unique<BinaryOpNode>(std::move(node), token.type, std::move(rightNode), lexer_inst.line_number);

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
            if(!symbolTableInstance.lookupSymbol(potentialVarName)) {
                throw std::runtime_error("Invalid Program : Use of undeclared variable " + potentialVarName +  " at line " +
                std::to_string(lexer_inst.line_number));
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
            throw std::runtime_error("Invalid Program : Invalid token in factor! " +  token.value +  " at line " +
                                          std::to_string(lexer_inst.line_number));
        }
    }

    void checkASTForOverflows(const std::vector<std::unique_ptr<astNode>>& nodes) {
        for (const auto& node : nodes) {
            try {
                evaluate(node.get());
            } catch (const std::overflow_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    std::vector<std::unique_ptr<astNode>> parseAST() {
        std::vector<std::unique_ptr<astNode>> nodes;
        symbolTableInstance.pushScope();
        // Keep parsing until we hit the end of file token.
        while (current_token_inst.type != TokenType::EOF_TOK) {
            auto node = statement();
            // Only push back non-null nodes.
            if (node) {
                nodes.push_back(std::move(node));
            }

            // Now expect a semicolon or end of file after a non-null statement.
            if (node != nullptr && current_token_inst.type != TokenType::SEMICOLON && current_token_inst.type != TokenType::EOF_TOK) {
                throw std::runtime_error("Invalid Program : Expected end of statement! at line " +
                std::to_string(lexer_inst.line_number));
            }

            // If it was a semicolon, move to the next token.
            if (current_token_inst.type == TokenType::SEMICOLON) {
                eat(TokenType::SEMICOLON);
            }
        }
        // After finishing parsing checking if main exists otherwise syntax error
        bool mainFound = false;
        for (auto &&node : nodes )
        {
            if(node.get()->getType() == "MainFunctionNode")
            {
                mainFound = true;
            }
        }
        if (mainFound)
        {
            return nodes;
        } else{
            throw std::runtime_error("Invalid Program : Main not found in line : " +
                                     std::to_string(lexer_inst.line_number));
        }
    }

    std::unique_ptr<astNode> declaration() {
        // Check to disallow global variables declaration
        auto type = current_token_inst.type;
        lastDataType = tokenTypeToString(current_token_inst.type); // Store the data type

        eat(current_token_inst.type);  // Eat the type token.

        auto varName = current_token_inst.value;
        if(current_token_inst.type != TokenType::IDENTIFIER)
        {
            throw std::runtime_error("Invalid Program : Expected identifier after "+ current_token_inst.value + " at line " +
            std::to_string(lexer_inst.line_number));
        }
        if (lexer_inst.getNextToken().type == TokenType::ASSIGNMENT) {
            throw std::runtime_error("Invalid Program : Direct assignment during declaration is not allowed at line " +
                                     std::to_string(lexer_inst.line_number));
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
                    symbolTableInstance.pushScope();
                    //std::vector<std::pair<std::string, std::string>> parameters;
                    std::vector<std::unique_ptr<DeclarationNode>> parameters;
                    if (current_token_inst.type != TokenType::R_PAREN) {
                        do{
                            // Expecting a type INT, CHAR, BOOL
                            if (current_token_inst.type == TokenType::INT ||
                                current_token_inst.type == TokenType::CHAR ||
                                current_token_inst.type == TokenType::FLOAT ||
                                current_token_inst.type == TokenType::BOOL) {
                                auto currentType = type();
                                std::string paramType = currentType;
                                //eat(current_token_inst.type);

                                // Expecting an identifier for the parameter name
                                if (current_token_inst.type != TokenType::IDENTIFIER) {
                                    throw std::runtime_error("Invalid Program : Expected parameter name at line " +
                                                             std::to_string(lexer_inst.line_number));
                                }
                                std::string paramName = current_token_inst.value;
                                eat(TokenType::IDENTIFIER);

                                // Add to parameters list
                                //parameters.emplace_back(paramType, paramName);
                                auto declarationNode = std::make_unique<DeclarationNode>(lastDataType,paramName);
                                symbolTableInstance.insertSymbol(paramName, declarationNode.get());
                                parameters.push_back(std::move(declarationNode));  // Add to parameters list

                                if (current_token_inst.type == TokenType::COMMA) {
                                    eat(TokenType::COMMA);
                                } else if (current_token_inst.type == TokenType::R_PAREN) {
                                    break;
                                } else {
                                    throw std::runtime_error("Invalid Program : Expected , or ) after parameter declaration at line " +
                                                             std::to_string(lexer_inst.line_number));
                                }
                            }
                            else {
                                return functionCall(varOrFuncName);
                            }
                        } while (current_token_inst.type != TokenType::R_PAREN);
                    }
                    eat(TokenType::R_PAREN);
                    if (current_token_inst.type != TokenType::L_BRACE) {
                        throw std::runtime_error("Invalid Program : Expected { after function definition at line " +
                                                 std::to_string(lexer_inst.line_number));
                    }

                    auto funcBody = blockStatement(true);
                    if (BlockNode* blockNode = dynamic_cast<BlockNode*>(funcBody.get())) {
                        if(varOrFuncName == tokenTypeToString(TokenType::MAIN))
                        {
                            auto functionNode = std::make_unique<MainFunctionNode>(varOrFuncName, std::move(blockNode->statements));
                            //symbolTable.insertGlobalSymbol(varOrFuncName, functionNode.get());
                            symbolTableInstance.insertSymbol(varOrFuncName,functionNode.get());
                            return functionNode;
                        }
                        else{
                            auto functionNode = std::make_unique<FunctionDeclarationNode>(varOrFuncName, std::move(parameters), std::move(blockNode->statements));
                            //symbolTable.insertGlobalSymbol(varOrFuncName, functionNode.get());
                            symbolTableInstance.insertSymbol(varOrFuncName,functionNode.get());
                            return functionNode;
                        }

                    } else {
                        throw std::runtime_error("Invalid Program : Expected a BlockNode for the function body at line " +
                                                 std::to_string(lexer_inst.line_number));
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
                        // Check if it's an array declaration or assignment
                        int arraySize = -1;  // Default, meaning not an array
                        if(current_token_inst.type == TokenType::L_BRACKET) {
                            eat(TokenType::L_BRACKET);
                            // Array declaration
                            if(current_token_inst.type == TokenType::NUMBER) {
                                arraySize = std::stoi(current_token_inst.value);
                                eat(TokenType::NUMBER);
                            }
                            // Array assignment
                            else if(current_token_inst.type == TokenType::IDENTIFIER) {
                                if(!symbolTableInstance.lookupSymbol(current_token_inst.value)) {
                                    throw std::runtime_error("Invalid Program : Use of undeclared variable " + current_token_inst.value + " at line " +
                                                             std::to_string(lexer_inst.line_number));
                                }
                                if(!(evaluate(symbolTableInstance.lookupSymbol(current_token_inst.value)) <= symbolTableInstance.getArraySize(varOrFuncName)))
                                {
                                    throw std::overflow_error("The program contains an integer overflow at line " +
                                                              std::to_string(lexer_inst.line_number) + ". The sum " +
                                                              " overflows the maximum array size.");
                                }
                                eat(TokenType::IDENTIFIER);
                            } else{
                                throw std::runtime_error("Invalid Program : Array size should be a number at line " +
                                std::to_string(lexer_inst.line_number));
                            }

                            if(current_token_inst.type != TokenType::R_BRACKET) {
                                throw std::runtime_error("Invalid Program : Expected closing bracket at line " +
                                std::to_string(lexer_inst.line_number));
                            }
                            eat(TokenType::R_BRACKET);
                        }
                        if (current_token_inst.type == TokenType::ASSIGNMENT) {
                            // Check if this variable exists in this scope
                            if(!symbolTableInstance.lookupSymbol(varOrFuncName)) {
                                throw std::runtime_error("Invalid Program : Use of undeclared variable " + varOrFuncName + " at line " +
                                std::to_string(lexer_inst.line_number));
                            }
                            eat(TokenType::ASSIGNMENT);
                            // Handle the right-hand side. It can be an expression, a function call, another variable, or a literal.
                            auto rightExpr = expr();
                            auto evaluatedValue = evaluate(rightExpr.get());
                            auto newValueNode = std::make_unique<NumberNode>(std::to_string(evaluatedValue));

                            auto rightExprCopy = deepCopyAstNode(rightExpr.get());

                            if(arraySize != -1) {
                                throw std::runtime_error("Invalid Program : Array initialization this way is not supported at line " +
                                std::to_string(lexer_inst.line_number));
                            }
                            variableNodes.push_back(std::make_unique<AssignmentNode>(std::make_unique<IdentifierNode>(varOrFuncName), std::move(rightExpr)));
                            symbolTableInstance.updateValueInScopes(varOrFuncName, std::move(newValueNode));
                            insideDeclarationScope=false;
                        }
                        else {
                            // It's just a variable declaration without an assignment
                            if(!insideDeclarationScope)
                            {
                                throw std::runtime_error("Invalid Program : Declaration must be at the beginning at line " +
                                std::to_string(lexer_inst.line_number));
                            }
                            if(arraySize != -1) {
                                variableNodes.push_back(std::make_unique<ArrayDeclarationNode>(lastDataType, varOrFuncName, arraySize));
                                symbolTableInstance.insertSymbol(varOrFuncName, variableNodes.back().get(),arraySize);  // Assuming ArrayDeclarationNode derives from astNode

                            } else {
                                variableNodes.push_back(std::make_unique<DeclarationNode>(lastDataType, varOrFuncName));
                                symbolTableInstance.insertSymbol(varOrFuncName, variableNodes.back().get());
                            }
                        }

                        if (current_token_inst.type == TokenType::COMMA) {
                            // Either normal assignment or function assignment
                            eat(TokenType::COMMA);
                            if (current_token_inst.type != TokenType::IDENTIFIER) {
                                throw std::runtime_error("Invalid Program : Expected another variable name after comma at line " +
                                std::to_string(lexer_inst.line_number));
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
                            throw std::runtime_error("Invalid Program : Expected comma, assignment, or semicolon at line " +
                            std::to_string(lexer_inst.line_number));
                        }
                    }

                    if (current_token_inst.type == TokenType::R_BRACE) { // R_CURLY means end of block
                        //symbolTable.popScope();
                    }
                    if (variableNodes.size() == 1) {
                        return std::move(variableNodes[0]);
                    }
                    return std::make_unique<CompoundNode>(std::move(variableNodes));
                }
                else {
                    throw std::runtime_error("Invalid Program : Unexpected token after IDENTIFIER at line " +
                                             std::to_string(lexer_inst.line_number));
                }
            }
            case TokenType::IF:
                symbolTableInstance.pushScope();
                return ifStatement();

            case TokenType::ELSE:
                symbolTableInstance.pushScope();
                return elseStatement();

            case TokenType::WHILE:
                symbolTableInstance.pushScope();
                return whileStatement();

            case TokenType::L_BRACE:
                symbolTableInstance.pushScope();
                return blockStatement(false);

            case TokenType::RETURN:
                return returnStatement();

            case TokenType::SEMICOLON:
                // Empty statement.
                if(current_token_inst.type==TokenType::SEMICOLON)
                {
                    throw std::runtime_error("Invalid Program : Unexpected token after SEMICOLON at line " +
                                             std::to_string(lexer_inst.line_number));      }
                return {};

            default:
                throw std::runtime_error("Invalid Program : This token is not defined in Grammar at line " +
                std::to_string(lexer_inst.line_number));

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
            auto newNode = std::make_unique<BinaryOpNode>(std::move(leftCopy), binaryOpNode->op, std::move(rightCopy), lexer_inst.line_number);
            return newNode;
        }
        else if (node->getType() == "Declaration") {
            const DeclarationNode* declNode = dynamic_cast<const DeclarationNode*>(node);
            auto valueCopy = deepCopyAstNode(declNode->value.get());
            auto newDeclNode = std::unique_ptr<DeclarationNode>(new DeclarationNode(declNode->dataType, declNode->name));
            newDeclNode->updateValue(std::move(valueCopy));
            return newDeclNode;
        }
        else if (node->getType() == "Compound") {
            const CompoundNode* compoundNode = dynamic_cast<const CompoundNode*>(node);
            std::vector<std::unique_ptr<astNode>> newNodes;
            for (const auto& child : compoundNode->nodes) {
                newNodes.push_back(deepCopyAstNode(child.get()));
            }
            return std::make_unique<CompoundNode>(std::move(newNodes));
        }
        else if (node->getType() == "IdentifierNode") {
            const IdentifierNode* idNode = dynamic_cast<const IdentifierNode*>(node);
            return std::make_unique<IdentifierNode>(idNode->value);
        }
        else if (node->getType() == "UnaryOperation") {
            const UnaryOpNode* unaryOpNode = dynamic_cast<const UnaryOpNode*>(node);
            auto operandCopy = deepCopyAstNode(unaryOpNode->operand.get());
            return std::make_unique<UnaryOpNode>(unaryOpNode->op, std::move(operandCopy));
        }
        else if (node->getType() == "FunctionCall") {
            const FunctionCallNode* functionCallNode = dynamic_cast<const FunctionCallNode*>(node);
            std::vector<std::unique_ptr<astNode>> argumentCopies;
            for (const auto& arg : functionCallNode->arguments) {
                argumentCopies.push_back(deepCopyAstNode(arg.get()));
            }
            return std::make_unique<FunctionCallNode>(functionCallNode->funcName, std::move(argumentCopies));
        }else if (node->getType() == "Boolean") {
            const BooleanNode* booleanNode = dynamic_cast<const BooleanNode*>(node);
            return std::make_unique<BooleanNode>(booleanNode->value);
        } else if (node->getType() == "FunctionDeclaration") {
            const FunctionDeclarationNode* funcDeclNode = dynamic_cast<const FunctionDeclarationNode*>(node);
            std::vector<std::unique_ptr<astNode>> bodyStatementsCopy;
            for (const auto& stmt : funcDeclNode->bodyStatements) {
                bodyStatementsCopy.push_back(deepCopyAstNode(stmt.get()));
            }
            // Deep copy parameters
            std::vector<std::unique_ptr<DeclarationNode>> parametersCopy;
            for (const auto& param : funcDeclNode->parameters) {
                auto paramCopy = deepCopyAstNode(param.get());
                if (DeclarationNode* declNode = dynamic_cast<DeclarationNode*>(paramCopy.get())) {
                    parametersCopy.push_back(std::unique_ptr<DeclarationNode>(declNode));
                } else {
                    throw std::runtime_error("Failed to deep copy parameter as DeclarationNode");
                }
            }
            return std::make_unique<FunctionDeclarationNode>(
                    funcDeclNode->funcName,
                    std::move(parametersCopy),
                    std::move(bodyStatementsCopy)
            );
        }
        else if (node->getType() == "Return") {
            const ReturnNode* returnNode = dynamic_cast<const ReturnNode*>(node);
            auto returnValueCopy = deepCopyAstNode(returnNode->returnValue.get());
            return std::make_unique<ReturnNode>(std::move(returnValueCopy));
        }
        else if (node->getType() == "MainFunctionNode") {
            const MainFunctionNode* mainFuncNode = dynamic_cast<const MainFunctionNode*>(node);
            std::vector<std::unique_ptr<astNode>> bodyStatementsCopy;
            for (const auto& stmt : mainFuncNode->bodyStatements) {
                bodyStatementsCopy.push_back(deepCopyAstNode(stmt.get()));
            }
            return std::make_unique<MainFunctionNode>(mainFuncNode->funcName, std::move(bodyStatementsCopy));
        }
        else if (node->getType() == "FunctionNode") {
            const FunctionNode* functionNode = dynamic_cast<const FunctionNode*>(node);
            std::vector<std::unique_ptr<astNode>> declarationsCopy, statementsCopy;
            for (const auto& decl : functionNode->declarations) {
                declarationsCopy.push_back(deepCopyAstNode(decl.get()));
            }
            for (const auto& stmt : functionNode->statements) {
                statementsCopy.push_back(deepCopyAstNode(stmt.get()));
            }
            return std::make_unique<FunctionNode>(functionNode->returnType, functionNode->name, functionNode->parameters, std::move(declarationsCopy), std::move(statementsCopy));
        }
        else if (node->getType() == "CharacterNode") {
            const CharacterNode* charNode = dynamic_cast<const CharacterNode*>(node);
            return std::make_unique<CharacterNode>(charNode->value);
        }
        else if (node->getType() == "Assignment") {
            const AssignmentNode* assignmentNode = dynamic_cast<const AssignmentNode*>(node);
            auto variableCopy = deepCopyAstNode(assignmentNode->variable.get());
            auto expressionCopy = deepCopyAstNode(assignmentNode->expression.get());
            return std::make_unique<AssignmentNode>(std::move(variableCopy), std::move(expressionCopy));
        } else if (node->getType() == "UnaryOperation") {
            const UnaryOpNode* unaryOpNode = dynamic_cast<const UnaryOpNode*>(node);
            auto operandCopy = deepCopyAstNode(unaryOpNode->operand.get());
            return std::make_unique<UnaryOpNode>(unaryOpNode->op, std::move(operandCopy));
        }
        else if (node->getType() == "FunctionCall") {
            const FunctionCallNode* functionCallNode = dynamic_cast<const FunctionCallNode*>(node);
            std::vector<std::unique_ptr<astNode>> argumentCopies;
            for (const auto& arg : functionCallNode->arguments) {
                argumentCopies.push_back(deepCopyAstNode(arg.get()));
            }
            return std::make_unique<FunctionCallNode>(functionCallNode->funcName, std::move(argumentCopies));
        } else if (node->getType() == "Variable") {
            const VariableNode* variableNode = dynamic_cast<const VariableNode*>(node);
            return std::make_unique<VariableNode>(variableNode->name);
        }
        else if (node->getType() == "Block") {
            const BlockNode* blockNode = dynamic_cast<const BlockNode*>(node);
            std::vector<std::unique_ptr<astNode>> statementsCopy;
            for (const auto& stmt : blockNode->statements) {
                statementsCopy.push_back(deepCopyAstNode(stmt.get()));
            }
            return std::make_unique<BlockNode>(std::move(statementsCopy));
        }else if (node->getType() == "IfStatement") {
            const IfNode* ifNode = dynamic_cast<const IfNode*>(node);
            auto conditionCopy = deepCopyAstNode(ifNode->condition.get());
            auto trueBranchCopy = deepCopyAstNode(ifNode->trueBranch.get());
            auto falseBranchCopy = ifNode->falseBranch ? deepCopyAstNode(ifNode->falseBranch.get()) : nullptr;
            return std::make_unique<IfNode>(std::move(conditionCopy), std::move(trueBranchCopy), std::move(falseBranchCopy));
        }
        else if (node->getType() == "WhileLoop") {
            const WhileNode* whileNode = dynamic_cast<const WhileNode*>(node);
            auto conditionCopy = deepCopyAstNode(whileNode->condition.get());
            auto loopBodyCopy = deepCopyAstNode(whileNode->loopBody.get());
            return std::make_unique<WhileNode>(std::move(conditionCopy), std::move(loopBodyCopy));
        }


        throw std::runtime_error("Unsupported node type to copy: " + node->getType());
        return nullptr;
    }

    std::unique_ptr<astNode> blockStatement(bool insideFunction) {
        // Allow local variables
        if(insideFunction)
        {
            insideFunctionScope=true;
        }
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
        if(insideFunction)
        {
            insideFunctionScope= false;
            insideDeclarationScope= true;
        }
        symbolTableInstance.popScope();
        return std::make_unique<BlockNode>(std::move(statements));
    }

    std::unique_ptr<astNode> returnStatement() {
        eat(TokenType::RETURN);
        auto returnValue = expr();
        if (current_token_inst.type != TokenType::SEMICOLON) {
            throw std::runtime_error("Invalid Program : Expected ; after return statement at line " +
            std::to_string(lexer_inst.line_number));
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
            throw std::runtime_error("Invalid Program : Expected else branch ! at line " +
                                     std::to_string(lexer_inst.line_number));
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

        return std::make_unique<MainFunctionNode>("main", std::move(stmts));
    }

    std::unique_ptr<astNode> functionCall(const std::string& funcName) {
        eat(TokenType::L_PAREN);
        std::vector<std::unique_ptr<astNode>> arguments;
        while (current_token_inst.type != TokenType::R_PAREN) {
            arguments.push_back(expr());
            if (current_token_inst.type == TokenType::COMMA) {
                eat(TokenType::COMMA);
            } else if (current_token_inst.type != TokenType::R_PAREN) {
                throw std::runtime_error("Invalid Program : Expected comma or right parenthesis in function call at line " +
                std::to_string(lexer_inst.line_number));
            }
        }
        eat(TokenType::R_PAREN);

        return std::make_unique<FunctionCallNode>(funcName, std::move(arguments));
    }

    std::unique_ptr<astNode> function() {
        // Parse the return type of the function
        auto returnType = type();

        // Parse the function name
        auto funcName = current_token_inst.value;
        eat(TokenType::IDENTIFIER);

        // Parse the parameter list
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
                throw std::runtime_error("Invalid Program : Expected , or ) in parameter list at line " +
                                         std::to_string(lexer_inst.line_number));
            }
        }
        eat(TokenType::R_PAREN);


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

        std::unique_ptr<ProgramNode> program = std::make_unique<ProgramNode>(std::vector<std::unique_ptr<astNode>>{});

        program->addFunction(std::move(mainFuncNode));

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
                throw std::runtime_error("Invalid Program : Invalid type! at line " +
                std::to_string(lexer_inst.line_number));
        }
    }

    std::string serializeAST(const std::vector<std::unique_ptr<astNode>>& astNodes) {
        std::string result = "[";
        for (size_t i = 0; i < astNodes.size(); ++i) {
            if (astNodes[i]) {
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