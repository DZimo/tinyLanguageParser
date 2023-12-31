
#include "main.h"
#include "headersGeneralizer.h"
#include "Syntax_checker/Script/parser.cpp"

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
    std::string script = readFile("src/script.txt");
    logger::initialize();
    lexer lex(script);
    parser parse(lex);
    imageGenerator generator;
    std::string generatedParserJson;

    try {
        // PARSE
        auto astNodes = parse.parseAST();
        generatedParserJson = parse.serializeAST(astNodes);
        // GENERATE IMAGE
        //auto bmpData = generator.astToBMP(astNodes);
        //generator.saveBMP("test.bmp",bmpData);

        std::string providedParserJson;
        std::ofstream generatedOut("generatedAST.json");
        generatedOut << generatedParserJson;
        generatedOut.close();
        std::cerr << "Valid program - " << std::endl;
        std::cout << "AST: " << generatedParserJson;

    } catch(const std::runtime_error& e) {
        std::cerr << "Parsing failed - " << e.what() << std::endl;
        logger::error(e.what());
    }

    logger::close();
    return 0;
}


