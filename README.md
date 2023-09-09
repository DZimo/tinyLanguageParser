# HOW TO RUN ?
Simply put your desired script in "src/script.txt" then compile the code g++ -o main src/main.cpp after that execute it with ./main 
you will get the results either in run prompt or log.txt if it is a valid you get the constructed AST in both JSON format and BMP format for image representation
otherwise in case it is an invalid program then you get the error.

# HOW IT WORKS ?
I wrote a Lexer and Parser from scratch depending on the provided EBNF grammar for this Tiny Programming Language
for the Lexer class...
For the AST construction I defined multiple Nodes which may be updated for more precise construction of AST, declaration of function, single,
multiple variables, if, else, while, and others.. which can be found in astNode class
JSON format convertor is based simply on those node and each node has it's own toJSON function that does the work
From JSON to BMP image i have implemented a class imageConvertor that takes the JSON then converts it simply to BMP format which can make it very useful and informative
BMP image still needs updates

# CHAR 
I assumed that characters works with '' instead of this ‘’ , example : 
c = ‘a’; ---> Invalid
c = 'a'; ---> Valid

# TESTS
I wanted to handle tests differently without including any library or framework for testing only for the sack of portability and ease of use
I made 2 classes one for valid and the other for invalid syntax as described in project, each class contains multiple functions that
have different scripts with one main function that call those tests, in a simple way i test with this script if the parseAST throws an error
or no, if yes then print what it expected to do, example : if a give an invalid script in the function then when it catches then the test passed
because it was expected to catch, I also print the exception.

# tinyLanguageParser
A parser for a fictional language called Tiny that is similar to C
Lexical Analysis:
We must first tokenize the script, then we start parsing token by token, the parsing will fail in the following cases:
-Declarations outside functions then the parsing fails as global variables are not allowed.
-Assignment for undeclared variables, after we check them in the current scope of symbol table.
-Another declaration after assignment, as the tiny has first declarations part then assignment part.
-Syntax error, like finishing a statement with semicolon ; or not following the rules for function declaration, parenthesis and other tokens.


Syntax Analysis:
We match each token with our grammar that describe Tiny Language, if a token doesn't match a specific rule then we output syntax error.
there is a case where we match multiple rules because of the ambiguity of the grammar, we also have to choose the ideal rule.

Semantic Analysis:
If the syntax analysis pass then we can start our semantic check, if for example we have "l"+1 then we output a semantic error

Overflow Analysis:
If an operation either addition,subtraction, multiplication, division or modulo exceeds the max int or min int, also same for buffers then we output overflow error with the details of the line and variables.
I assumed that we have only unsigned numbers, basically 16 bits is equal to a max_int of 65536.
After getting the AST we try to go node by node and whenever we have an binary operation node then we check the right value and left value with the given operation
to check if it overflows or no.

Output:
if all analysis pass then we output valid program with the AST.

# VERSIONS
C++ 17
CMAKE 3.25
G++ 13.2

# MOHAMED-NAZIM BELABED 110858
