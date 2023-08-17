# HOW TO RUN ?
Simply put your desired script in "src/script.txt" then compile the code g++ -o src/main after that execute it ./main 
you will get either in run prompt or log.txt: if it is a valid or invalid program then the constructed AST in both JSON format and Image

# HOW IT WORKS ?
I wrote a Lexer and Parser from scratch depending on the provided EBNF grammar for this Tiny Programming Language
for the Lexer class...
For the AST i defined multiple Nodes which may be updated for more precise construction of AST, declaration of function, single,
multiple variables, if, else, while, and others.. which can be found in astNode class
JSON format convertor is based simply on those node and each node has it's own toJSON function that does the work
From JSON to image i have used a library that takes the JSON then converts it simply to PNG format which can make it very useful and informative

# TESTS
I wanted to handle tests differently without including any library or framework for testing only for the sack of portability and ease of use
i made 2 classes one for valid and the other for invalid syntax as described in project, each class contains multiple functions that
have different scripts with one main function that call those tests, in a simple way i test with this script if the parseAST throws an error
or no, if yes then print what it expected to do, example : if a give an invalid script in the function then when it catch then the test passed
because it was expected to catch, i also print the exception.

# tinyLanguageParser
A parser for a fictional language called Tiny that is similar to C
Lexical Analysis:
We must first tokenize the script, first thing if the first token is not int main() then we output invalid program because global variables are not allowed.
Syntax Analysis:
Then we match each token with our grammar that describe Tiny Language, if a token doesn't match a specific rule then we output syntax error.
there is a case where we match multiple rules because of the ambigiousity of the grammar, we also have to choose the ideal rule.
Semantic Analysis:
If the syntax analysis pass then we can start our semantic check, if for example we have "l"+1 then we output a semantic error
Overflow Analysis:
If an addition or subtraction exceds the max int or min int, also same for buffers then we output overflow error with the details of the line and variables.
Output:
if all analysis pass then we output valid program.

# VERSIONS
C++ 17
CMAKE 3.25
G++ 13.2

# MOHAMED-NAZIM BELABED 110858
