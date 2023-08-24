
#include "main.h"
#include "Syntax_checker/EBNF/EBNFparser.h"
#include "Syntax_checker/Script/parser.cpp"
#include <iostream>
#include <string>
#include <fstream>
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

std::string serializeWithGeneratedSerializer(const std::vector<std::unique_ptr<astNode>>& astNodes) {
    std::string serialized = "[";

    for (size_t i = 0; i < astNodes.size(); ++i) {
        serialized += astNodes[i]->toJSON();

        if (i < astNodes.size() - 1) {
            serialized += ",";
        }
    }

    serialized += "]";
    return serialized;
}

int main() {
    //std::string script = "main ( int b, int c ) { int a; int b; float d; bool s;} f () { int c;} c() {}  a = c + d;";
    std::string script = readFile("src/script.txt");
    lexer lex(script);
    parser parse(lex);
    std::string generatedParserJson;
    try {
        auto astNodes = parse.parseAST();
        generatedParserJson = parse.serializeAST(astNodes);
    } catch(const std::runtime_error& e) {
        std::cerr << "Parsing failed - " << e.what() << std::endl;
    }
    std::string ebnfContent = readFile("EBNF_grammar_Tiny.txt");
    std::string providedParserJson;
    std::ofstream generatedOut("generatedAST.json");
    generatedOut << generatedParserJson;
    generatedOut.close();
    std::cout << "Provided parser output (first 100 chars): " << generatedParserJson;

    return 0;
}
