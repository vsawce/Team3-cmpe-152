#include <iostream>
#include <fstream>
#include <list>
#include "antlr4-runtime.h"
#include "target/generated-sources/antlr4/ExprLexer.h"
#include "target/generated-sources/antlr4/ExprParser.h"
#include "symtab/SymtabStack.h"
#include "symtab/Typespec.h"
using namespace antlrcpp;
using namespace antlr4;
using namespace std;

template <typename Kind>
constexpr auto kind_to_underlying(Kind k) noexcept
{
    return static_cast<std::underlying_type_t<Kind>>(k);
}

template <typename Form>
constexpr auto form_to_underlying(Form f) noexcept
{
    return static_cast<std::underlying_type_t<Form>>(f);
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

void lispToSymtab(ifstream &insLisp, ofstream &outsSt) {
    char c;
    intermediate::symtab::SymtabStack *sts = new intermediate::symtab::SymtabStack();
    intermediate::symtab::Symtab *st = sts->getLocalSymtab();
    //int level = -1;
    //std::string xmlString = "";
    std::string token = "";
    std::string prevToken = "";
    intermediate::symtab::Kind kind;
    intermediate::type::Form form;
    bool nextIsIdentifier = false;
    bool nextIsType = false;
    //bool firstToken = false;
    //list<std::string> strStack;

    while (insLisp >> noskipws >> c) { // Not EOF
        if (token == "programHeader") {
            kind = intermediate::symtab::Kind::PROGRAM;
        }
        else if (token == "procedureDeclarationHeader" && c == ' ') {
            kind = intermediate::symtab::Kind::PROCEDURE;
            //New table
            //st = sts->enterLocal(token);
        }
        else if (token == "functionDeclarationHeader" && c == ' ') {
            kind = intermediate::symtab::Kind::FUNCTION;
            //New table
            //sts->push();
            //st = sts->enterLocal(token);
        }
        else if (token == "variableDeclaration" && c == ' ') {
            kind = intermediate::symtab::Kind::VARIABLE;
        }
        else if (token == "formalParameterSection" && c == ' ') {
            kind = intermediate::symtab::Kind::VALUE_PARAMETER;
        }
        else if (token == "VAR" && prevToken == "formalParameterSection" && c == ' ') {
            kind = intermediate::symtab::Kind::REFERENCE_PARAMETER;
        }
        else if (token == "constantDeclaration" && c == ' ') {
            kind = intermediate::symtab::Kind::CONSTANT;
        } 
        else if (token == "typeDeclaration" && c == ' ') {
            kind = intermediate::symtab::Kind::TYPE;
        }
        else if (token == "identifier" && c == ' ') {
            nextIsIdentifier = true;
            if (prevToken == "typeIdentifier") { //Then assign type
                nextIsType = true;
            }
        }
        if (c == '(') {
        }
        else if (c == ')') {
            if (nextIsIdentifier) { //Add identifier to table
                intermediate::symtab::SymtabEntry *ste;
                nextIsIdentifier = false;                
                if (st->lookup(token) == nullptr) //Entry does not exist
                    ste = st->enter(token, kind); //Kind Variable for now
                kind = intermediate::symtab::Kind::UNDEFINED;

                if (nextIsType) { //Add type to table
                    if (token == "integer") {
                        form = intermediate::type::Form::SCALAR;
                    }
                    intermediate::type::Typespec ts(form);
                    nextIsType = false;                
                    ste->setType(&ts);
                }
            }
        }
        else if (c == ' ') {
            prevToken = token;
            token = "";
        }
        else {
            token += c;
        }
        //append to list
        //cout << token << endl;
    }
    //vector<SymtabEntry *> sortedE = st->sortedEntries();
    for (int i = 0; i < st->sortedEntries().size(); i++) { //For each entry
        std::string typestring = "NULL";
        if (st->sortedEntries()[i]->getKind() == intermediate::symtab::Kind::VARIABLE) {
            //WORK HERE
            intermediate::type::SymtabEntry *typestring = st->sortedEntries()[i]->getType()->getIdentifier();
            outsSt << st->sortedEntries()[i]->getName() << " " << kind_to_underlying(st->sortedEntries()[i]->getKind()) << " " << typestring << endl;
            //underneath is the old code
            intermediate::type::Typespec *poopform = st->sortedEntries()[i]->getType();
            int poop = form_to_underlying(poopform->getForm()); //Equals 1433512416 for some reason, should be < 5
            intermediate::type::FORM_STRINGS[ form_to_underlying(st->sortedEntries()[i]->getType()->getForm()) ];
            //typestring = intermediate::type::FORM_STRINGS[form_to_underlying(st->sortedEntries()[i]->getType()->getForm())];
        }
        
        //form_to_underlying(st->sortedEntries()[i]->getType()->getForm());
        //outsSt << st->sortedEntries()[i]->getName() << " " << kind_to_underlying(st->sortedEntries()[i]->getKind()) << " " << typestring << endl;
        outsSt << i << " " << st->sortedEntries()[i]->getName() << "\t\t" << intermediate::symtab::KIND_STRINGS[kind_to_underlying(st->sortedEntries()[i]->getKind())] << "\t\t" << typestring << /*intermediate::type::FORM_STRINGS[form_to_underlying(st->sortedEntries()[i]->getType()->getForm())] << */endl;
    }
}

void generateSicXe(ifstream &insSymTab, ifstream &insLisp, ofstream &outsSX) {
    char c;
    intermediate::symtab::SymtabStack *sts = new intermediate::symtab::SymtabStack();
    intermediate::symtab::Symtab *st = sts->getLocalSymtab();
    //int level = -1;
    //std::string xmlString = "";
    /*
    for (int i = 0; i < st->sortedEntries().size(); i++) { //For each entry
        outsSX << i << " " << st->sortedEntries()[i]->getName() << "\t\t" << intermediate::symtab::KIND_STRINGS[kind_to_underlying(st->sortedEntries()[i]->getKind())] << "\t\t"  <<endl;
    }
    */
}

int main(int argc, const char *args[]){
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

/*int main(int argc, const char *args[])
{
std::string outFile = "test-out.txt";
std::string lispFile = "test-out-tree-lisp.txt";
std::string xmlFile = "test-out-tree.xml";
std::string symtabFile = "test-out-symtab.txt";
std::string sicXeFile = "test-out-sicxe.asm";

ifstream ins;
ofstream outs(outFile);
ofstream outsLisp(lispFile);
ofstream outsXml(xmlFile);
ofstream outsSymtab(symtabFile);
ofstream outsSicXe(sicXeFile);
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

cout << endl << "-----" << endl;
// Print the parse tree in Lisp format.
outs << endl << "Parse tree (Lisp format):" << endl;
outs << tree->toStringTree(&parser) << endl;
outsLisp << tree->toStringTree(&parser) << endl;

ifstream insLisp(lispFile);
ifstream insLisp2(lispFile);
ifstream insLisp3(lispFile);
ifstream insSymTab(symtabFile);

outsXml << lispToXml(insLisp) << endl;
outs << endl;
lispToSymtab(insLisp2, outsSymtab);
generateSicXe(insSymTab, insLisp3, outsSicXe);

cout << "Program complete, check: " << outFile << ", " << lispFile << ", " << xmlFile << ", " << symtabFile << ", " << sicXeFile << endl;
return 0;
}*/
