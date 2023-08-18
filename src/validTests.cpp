
#include "validTests.h"
#include "main.h"
#include "Syntax_checker/EBNF/EBNFparser.h"
#include "Syntax_checker/Script/parser.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


void randomValidTest ()
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

void programAValidTest ()
{
    std::string script = "int main () {\n"
                         "bool x;\n"
                         "float y, z;\n"
                         " z = 0;\n"
                         " y = 0;\n"
                         "x = true;\n"
                         "if(x) {\n"
                         "y = y + z;\n"
                         "}\n"
                         "else{\n"
                         "y = y - z;\n"
                         "}\n"
                         "return 0;\n"
                         "}";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
        std::cout << "Test Passed ( programAValidTest ) " << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Test Failed ( programAValidTest ) : " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

void programBValidTest ()
{
    std::string script = "int main () {\n"
                         "int i;\n"
                         " char c;\n"
                         "i = 20;\n"
                         "while (i > 0)\n"
                         "{c = ‘a’;\n"
                         "}\n"
                         "return 0;\n"
                         "}";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
        std::cout << "Test Passed ( programBValidTest ) " << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Test Failed ( programBValidTest ) : " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

void programCValidTest ()
{
    std::string script = "int sum(int a, int b) {\n"
                         "int c;\n"
                         "a = 10;\n"
                         "b = 20;\n"
                         "c = a + b;\n"
                         "return c;\n"
                         "}\n"
                         "int main () {\n"
                         "int a, b, c;\n"
                         "a = 10;\n"
                         "b = 20;\n"
                         "c = sum(a, b);\n"
                         "return 0;\n"
                         "}";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
        std::cout << "Test Passed ( programCValidTest ) " << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Test Failed ( programCValidTest ) : " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

void programDValidTest ()
{
    std::string script = "int main() {\n"
                         " int a[10], i;\n"
                         " i = 10;\n"
                         " while(i > 0) {\n"
                         " a[i] = i;\n"
                         " }\n"
                         " return 0; \n"
                         "}";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
        std::cout << "Test Passed ( programDValidTest ) " << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Test Failed ( programDValidTest ) : " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}


int main ()
{
    randomValidTest  ();
    programAValidTest();
    programBValidTest();
    programCValidTest();
    programDValidTest();
}

