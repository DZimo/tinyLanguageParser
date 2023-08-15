
#include "main.h"
#include "Syntax_checker/parser.cpp"

#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <sstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    /*
    std::string script = readFile("src/script.txt");
    lexer lexer(script);

    tokenizer token = lexer.getNextToken();
    while (token.getType() != TokenType::EOF_TOK) {
        token.print();
        token = lexer.getNextToken();
    }
    lexer.getSymbolTable().print();
    return 0;*/

    std::string script = "main () { int a; int b; }";

    //std::string script = readFile("src/script.txt");

    // Initialize the lexer with the script
    lexer lex(script);

    // Initialize the parser with the lexer
    parser parse(lex);
    // Parse the script to build AST
    try {
        auto astNodes = parse.parseAST();
        for (const auto& node : astNodes) {
            node->print(std::cout);
            std::cout << std::endl;
        }
        // print json string
        std::string json = parse.serializeAST(astNodes);
        std::cout << json << std::endl;

    } catch(const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    return 0;
}
