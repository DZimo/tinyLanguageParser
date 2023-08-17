cmake_minimum_required(VERSION 3.25)
project(tinyLanguageParser)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tinyLanguageParser
        src/main.cpp
        src/main.h src/main.cpp src/Lexical_checker/tokenizer.h src/Lexical_checker/tokenizer.h src/Lexical_checker/tokenizer.cpp src/Lexical_checker/lexer.h src/Lexical_checker/lexer.cpp src/SymbolTable_checker/symbolTable.h src/SymbolTable_checker/symbolTable.cpp src/Syntax_checker/Script/parser.h src/Syntax_checker/Script/parser.cpp src/AST_checker/astNode.h src/AST_checker/astNode.cpp src/Syntax_checker/programNode.h src/Syntax_checker/programNode.h src/Syntax_checker/programNode.cpp src/Syntax_checker/EBNF/EBNFchecker.h src/Syntax_checker/EBNF/EBNFchecker.cpp src/Syntax_checker/EBNF/EBNFchecker.cpp src/Syntax_checker/EBNF/EBNFparser.h src/Syntax_checker/EBNF/EBNFparser.cpp src/Lexical_checker/EBNFlexer.h src/Lexical_checker/EBNFlexer.cpp tests/invalidTests.h tests/invalidTests.cpp tests/validTests.cpp)