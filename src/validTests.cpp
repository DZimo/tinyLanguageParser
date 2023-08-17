
#include "validTests.h"
#include "main.h"
#include "Syntax_checker/EBNF/EBNFparser.h"
#include "Syntax_checker/Script/parser.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


void firstValidTest ()
{
    std::string script = "main ( int b, int c ) { int a; int b; float d; bool s;} f () { int c;} c() {}  a = c + d;";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
        std::cout << "Test Passed ( firstValidTest ) " << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Test Failed ( firstValidTest ) : " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

int main ()
{
    firstValidTest ();
}

