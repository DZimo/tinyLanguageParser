cmake_minimum_required(VERSION 3.25)
project(tinyLanguageParser)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tinyLanguageParser
        src/main.cpp
        src/main.h src/main.cpp src/Lexical_checker/tokenizer.h src/Lexical_checker/tokenizer.h src/Lexical_checker/tokenizer.cpp src/Lexical_checker/lexer.h src/Lexical_checker/lexer.cpp src/SymbolTable_checker/symbolTable.h src/SymbolTable_checker/symbolTable.cpp src/Syntax_checker/parser.h src/Syntax_checker/parser.cpp src/AST_checker/astNode.h src/AST_checker/astNode.cpp src/AST_checker/assignementNode.h src/AST_checker/assignementNode.cpp src/AST_checker/variableNode.h src/AST_checker/variableNode.cpp src/AST_checker/declarationNode.h src/AST_checker/declarationNode.cpp src/Syntax_checker/programNode.h src/Syntax_checker/programNode.h src/Syntax_checker/programNode.cpp src/Syntax_checker/EBNFchecker.h src/Syntax_checker/EBNFchecker.cpp src/Syntax_checker/EBNFchecker.cpp src/Syntax_checker/EBNFparser.h src/Syntax_checker/EBNFparser.cpp src/Lexical_checker/EBNFlexer.h src/Lexical_checker/EBNFlexer.cpp)