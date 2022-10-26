#include <iostream>
#include <fstream>
#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"
#include "symtab/Predefined.h"
using namespace antlrcpp;
using namespace antlr4;
using namespace std;
int main(int argc, const char *args[])
{
std::string outFile = "test-out.txt";

ifstream ins;
ofstream outs(outFile);
// Create the input stream.
ins.open(args[1]);
// outs.open(outFile);
ANTLRInputStream input(ins);
// Create a lexer which scans the input stream
// to create a token stream.
ExprLexer lexer(&input);
CommonTokenStream tokens(&lexer);
// Print the token stream.
outs << "Tokens:" << endl;
tokens.fill();
for (Token *token : tokens.getTokens())
{
outs << token->toString() << std::endl;
}
// Create a parser which parses the token stream
// to create a parse tree.
ExprParser parser(&tokens);
tree::ParseTree *tree = parser.program();
// Print the parse tree in Lisp format.
outs << endl << "Parse tree (Lisp format):" << endl;
outs << tree->toStringTree(&parser) << endl;
cout << "Program complete, check: " << outFile << endl;
return 0;
}
