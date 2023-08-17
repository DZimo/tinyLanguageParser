
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

    //std::string script = "main ( int b, int c ) { int a; int b; float d; bool s;} f () { int c;} c() {}  a = c + d;";
    std::string script = readFile("src/script.txt");
    // Using the provided parser
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
        providedParserJson = parse.serializeAST(astNodes);
    } catch(const std::runtime_error& e) {
        std::cerr << "Error with provided parser: " << e.what() << std::endl;
    }

    // Read EBNF content from file (assuming you have a readFile function)
    std::string ebnfContent = readFile("EBNF_grammar_Tiny.txt");
    std::string generatedParserJson;

    // Using the generated parser
    // Step 2: Parse the EBNF content
    std::vector<std::unique_ptr<astNode>> ast;
    try {
        //ast = parseWithGeneratedParser(ebnfContent);
        //generatedParserJson = serializeWithGeneratedSerializer(ast);

    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
        return 2;
    }
    // Save to files
    std::ofstream generatedOut("generated_parser_output.json");
    generatedOut << generatedParserJson;
    generatedOut.close();

    std::ofstream providedOut("provided_parser_output.json");
    providedOut << providedParserJson;
    providedOut.close();

    // Quick debug: print the first few characters of both JSON strings
    std::cout << "Provided parser output (first 100 chars): " << providedParserJson;
    std::cout << "Generated parser output (first 100 chars): " << generatedParserJson.substr(0, 100) << "\n";

    // Compare the two JSON outputs
    if (providedParserJson == generatedParserJson) {
        std::cout << "Both parsers produced the same AST!\n";
    } else {
        std::cout << "The parsers produced different ASTs!\n";
    }

    return 0;
}
