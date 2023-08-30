
#include "Syntax_checker/EBNF/EBNFparser.h"
#include "Syntax_checker/Script/parser.cpp"
#include <iostream>
#include <string>
#include <sstream>


void randomInvalidTest ()
{
    std::string script = "int a; main ( int b, int c ) { int a; int b; float d; bool s;} f () { int c;} c() {int a; int c; int d; a = 1;}  ";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
    } catch (const std::runtime_error& e) {
        std::string excpectedException = "Invalid Program : Global variables not allowed";
        if(e.what()== excpectedException)
        {
            std::cout << "Test Passed ( programBInvalidTest ) : " << e.what() << std::endl;
        } else std::cout << "Test Failed ( programBInvalidTest ) " << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

void programAInvalidTest ()
{
    std::string script = "int main () {\n"
                         " int i;\n"
                         "i = 20;\n"
                         "char c; //error: declara-on must be at the beginning\n"
                         "return 0;\n"
                         "}";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
        std::cout << "Test Failed ( programAInvalidTest ) " << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Test Passed ( programAInvalidTest ) : " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

void programBInvalidTest ()
{
    std::string script = "int main () {\n"
                         "int a = 2; //error; declara-on should be in next lines\n"
                         "return 0;\n"
                         "}";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
    } catch (const std::runtime_error& e) {
        std::string excpectedException = "Invalid Program : Direct assignment during declaration is not allowed";
        if(e.what()== excpectedException)
        {
            std::cout << "Test Passed ( programBInvalidTest ) : " << e.what() << std::endl;
        } else std::cout << "Test Failed ( programBInvalidTest ) " << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

void programCInvalidTest ()
{
    std::string script = "int main () {\n"
                         "int c;\n"
                         " c = a; // error a not defined.\n"
                         " Int a; // not allowed here\n"
                         "return 0;\n"
                         "}";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
    } catch (const std::runtime_error& e) {
        std::string excpectedException = "Invalid Program : Use of undeclared variable a";
        if(e.what()== excpectedException)
        {
            std::cout << "Test Passed ( programCInvalidTest ) : " << e.what() << std::endl;
        } else std::cout << "Test Failed ( programCInvalidTest ) " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}

void programDInvalidTest ()
{
    std::string script = "int x; // global not allowed\n"
                         "int main(){x = 3;\n"
                         "return 0; }";
    lexer lex(script);
    parser parse(lex);
    std::string providedParserJson;
    try {
        auto astNodes = parse.parseAST();
    } catch (const std::runtime_error& e) {
        std::string excpectedException = "Invalid Program : Global variables not allowed";
        if(e.what()== excpectedException)
        {
            std::cout << "Test Passed ( programDInvalidTest ) : " << e.what() << std::endl;
        } else std::cout << "Test Failed ( programDInvalidTest ) " << e.what() << std::endl;
    }
    //if("[{ \"type\": \"FunctionDeclaration\", \"name\": \"main\", \"parameters\": [{ \"type\": \"int\", \"name\": \"b\" }, { \"type\": \"int\", \"name\": \"c\" }], \"body\": [{ \"type\": \"Declaration\", \"name\": \"a\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"b\", \"dataType\": \"INT\" }, { \"type\": \"Declaration\", \"name\": \"d", \"dataType\": \"FLOAT" }, { "type": "Declaration", "name": "s", "dataType": "BOOL" }] }, { "type": "FunctionDeclaration", "name": "f", "parameters": [], "body": [{ "type": "Declaration", "name": "c", "dataType": "INT" }] }, { "type": "FunctionDeclaration", "name": "c", "parameters": [], "body": [] }, { "type": "Assignment", "left": { "type": "IdentifierNode", "value": "a" }, "right": { "type": "BinaryExpression", "operator": "ADD_OP", "left": { "type": "Variable", "name": "c" }, "right": { "type": "Variable", "name": "d" } } }]"
}


int main ()
{
    randomInvalidTest  ();
    programAInvalidTest();
    programBInvalidTest();
    programCInvalidTest();
    programDInvalidTest();
}