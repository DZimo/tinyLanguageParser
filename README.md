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
