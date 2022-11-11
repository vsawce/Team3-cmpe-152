#include <iostream>
#include <fstream>
#include <list>
#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"
#include "symtab/Predefined.h"
using namespace antlrcpp;
using namespace antlr4;
using namespace std;

std::string lispToXml(ifstream &insLisp) {
    char c;
    int level = -1;
    std::string xmlString = "";
    std::string token = "";
    bool firstToken = false;
    list<std::string> strStack;

    while (insLisp >> noskipws >> c) { // Not EOF
        bool levelChange = false;
        if (c == '(') {
            firstToken = true;
            level++;
            levelChange = true;
            xmlString += "\n";
        }
        else if (c == ')') {
            level--;
            levelChange = true;
            xmlString += "</" + strStack.back() + ">\n";
            strStack.pop_back();
            //xmlString += "\n";
        }
        else if (c == ' ') {
            for (int i = 0; i < level; i++) {
                xmlString += "\t";
            }
            if (firstToken) {
                xmlString += "<" + token + ">";
                strStack.push_back(token);
                firstToken = false;
            }
            else {
                xmlString += token;
            }
            token = "";
        }
        else {
            token += c;
        }
        if (levelChange) {
            //xmlString += "\n";
        }
        //append to list
    }
    return xmlString;
}

int main(int argc, const char *args[])
{
std::string outFile = "test-out.txt";
std::string lispFile = "test-out-tree-lisp.txt";
std::string xmlFile = "test-out-tree-xml.xml";

ifstream ins;
ofstream outs(outFile);
ofstream outsLisp(lispFile);
ofstream outsXml(xmlFile);
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
outsLisp << tree->toStringTree(&parser) << endl;

ifstream insLisp(lispFile);

outsXml << lispToXml(insLisp) << endl;

cout << "Program complete, check: " << outFile << ", " << lispFile << ", " << xmlFile << endl;
return 0;
}
