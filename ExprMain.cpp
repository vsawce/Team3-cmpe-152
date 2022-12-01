#include <iostream>
#include <fstream>
#include <list>
#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"
#include "symtab/SymtabStack.h"
using namespace antlrcpp;
using namespace antlr4;
using namespace std;

template <typename Kind>
constexpr auto to_underlying(Kind k) noexcept
{
    return static_cast<std::underlying_type_t<Kind>>(k);
}

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
            levelChange = true;
            xmlString += token + "\n";
            for (int i = 0; i < level; i++) {
                xmlString += "\t";
            }
            level--;
            xmlString += "</" + strStack.back() + ">\n";
            token = "";
            strStack.pop_back();
            xmlString += token;
        } //xmlString += "\n";
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
            for (int i = 0; i < level; i++) {
                xmlString += "\t";
            }
            //xmlString += "\n";
        append(xmlString, token);
        //append to list
    }
    return xmlString;
}

void lispToSymtab(ifstream &insLisp, ofstream &outsSymtab) {
    char c;
    intermediate::symtab::SymtabStack *sts = new intermediate::symtab::SymtabStack();
    intermediate::symtab::Symtab *st = sts->getLocalSymtab();
    std::string token = "";
    bool firstToken = false;
    list<std::string> strStack;
    //int level = -1;
    //std::string xmlString = "";
    std::string token = "";
    intermediate::symtab::Kind kind;
    bool isType = false;
    /*bool nextIsIdentifier = false;
    bool nextIsType = false;
    bool nextIsKind = false;
    bool nextIsValue = false;
    bool nextIsRoutine = false;
    bool nextIsParameter = false;
    bool nextIsLocal = false;
    bool nextIsField = false;
    bool nextIsFunction = false;
    bool nextIsProcedure = false;
    bool nextIsVariable = false;
    bool nextIsConstant = false;
    bool nextIsTypeSpec = false;
    bool nextIsRecord = false;
    bool nextIsArray = false;
    bool nextIsSubrange = false;
    bool nextIsEnumeration = false;
    bool nextIsScalar = false;
    bool nextIsString = false;
    bool nextIsInteger = false;
    bool nextIsReal = false;
    bool nextIsBoolean = false;
    bool nextIsChar = false;
    bool nextIsValueRange = false;
    bool nextIsValueList = false;
    bool nextIsValueRangeList = false;*/
    //bool firstToken = false;
    list<std::string> strStack;
    //list<std::string> strStack;

    while (insLisp >> noskipws >> c) { // Not EOF
        bool levelChange = false;
        if (token == "programHeader") {
            kind = intermediate::symtab::Kind::PROGRAM;
        }
        else if (token == "procedureDeclarationHeader" && c == ' ') {
            kind = intermediate::symtab::Kind::FUNCTION;
            //New table
        }
        else if (token == "functionDeclarationHeader" && c == ' ') {
            kind = intermediate::symtab::Kind::FUNCTION;
            //New table
        }
        else if (token == "variableDeclaration" && c == ' ') {
            kind = intermediate::symtab::Kind::VARIABLE;
        }
        else if (token == "constantDeclaration" && c == ' ') {
            kind = intermediate::symtab::Kind::CONSTANT;
        } 
        else if (token == "typeDeclaration" && c == ' ') {
            kind = intermediate::symtab::Kind::TYPE;
        }
        else if (token == "identifier" && c == ' ') {
            nextIsIdentifier = true;
        }
        if (c == '(') {
        }
        else if (c == ')') {
            if (nextIsIdentifier) {
                nextIsIdentifier = false;                
                if (st->lookup(token) == nullptr) //Entry does not exist
                    st->enter(token, kind); //Kind Variable for now
                kind = intermediate::symtab::Kind::UNDEFINED;
            }
        }
        else if (c == ' ') {
            token = "";
        }
        else {
            token += c;
        }
        append(xmlString, token);
        //append to list
        cout << xmlString << endl;
        //cout << token << endl;
    }
    vector<intermediate::symtab::SymtabEntry *> entries = st->sortedEntries();
    for (intermediate::symtab::SymtabEntry *entry : entries) {
        outsSymtab << entry->getName() << " " << to_underlying(entry->getKind()) << endl;
    } //vector<SymtabEntry *> sortedE = st->sortedEntries();
    for (int i = 0; i < st->sortedEntries().size(); i++) { //For each entry
        outsSymtab << i << " " << st->sortedEntries()[i]->getName() << "\t\t" << intermediate::symtab::KIND_STRINGS[to_underlying(st->sortedEntries()[i]->getKind())] << "\t\t"  <<endl;
    }
}

int main(int argc, const char *args[])
{
std::string outFile = "test-out.txt";
std::string lispFile = "test-out-tree-lisp.txt";
std::string xmlFile = "test-out-tree.xml";

ifstream ins;
ofstream outs(outFile);
ofstream outsLisp(lispFile);
ofstream outsXml(xmlFile);
ofstream outsSymtab("test-out-symtab.txt");
// Create the input stream.
ins.open(args[1]);
if (ins.fail()) {
    cerr << "***** ERROR: Cannot open source file " << args[1] << endl;
    exit(-1);
}
// outs.open(outFile);
ANTLRInputStream input(ins);
// Create an input character stream from standard in
// ANTLRInputStream input(cin);
// Create an ExprLexer that feeds from that stream
ExprLexer lexer(&input);
// Create a stream of tokens fed by the lexer
CommonTokenStream tokens(&lexer);
// Create a parser that feeds off the token stream
ExprParser parser(&tokens);
// Create a tree walker that feeds off the parser's tree
tree::ParseTreeWalker walker;
//tree::ParseTree *tree = parser.program();
// Walk the tree created during the parse, trigger callbacks
// on entry to and exit from each rule.
//ExprListener listener;
//walker.walk(&listener, parser.program());
// Create a lexer which scans the input stream
// to create a token stream.
CommonTokenStream tokens(&lexer);
// Print the token stream.
//tokens.fill();
//for (Token *token : tokens.getTokens()) {
//    cout << token->toString() << endl;
//}
outs << "Tokens:" << endl;
tokens.fill();
for (Token *token : tokens.getTokens())
{
outs << token->toString() << std::endl;
}
cout << endl << "-----" << endl;
// Print the parse tree in Lisp format.
outsLisp << "Parse tree:" << endl;
outsLisp << parser.program()->toStringTree(&parser) << endl;
cout << endl << "-----" << endl;
// Print the parse tree in XML format.
outsXml << "Parse tree:" << endl;
outsXml << parser.program()->toStringTree(&parser) << endl;
cout << endl << "-----" << endl;
// Print the symbol table.
outsSymtab << "Symbol table:" << endl;
outsSymtab << parser.program()->toStringTree(&parser) << endl;
cout << endl << "-----" << endl;

/*outs << endl << "Parse tree (Lisp format):" << endl;
outs << tree->toStringTree(&parser) << endl;
outsLisp << tree->toStringTree(&parser) << endl;*/

ifstream insLisp(lispFile);
ifstream insLisp2(lispFile);
ifstream insXml(xmlFile);
//ofstream outsSymtab("test-out-symtab.txt");

outsXml << lispToXml(insLisp) << endl;
outs << endl;
lispToSymtab(insLisp2, outs);
//lispToSymtab(insLisp2, outsSymtab);

cout << "Program complete, check: " << outFile << ", " << lispFile << ", " << xmlFile << endl;
return 0;
}
