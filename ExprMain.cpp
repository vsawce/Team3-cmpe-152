#include <iostream>
#include <fstream>
#include <list>
#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"
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
        }
        else if (token == "functionDeclarationHeader" && c == ' ') {
            kind = intermediate::symtab::Kind::FUNCTION;
            //New table
            //sts->push();
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
                    intermediate::type::Typespec *ts = new intermediate::type::Typespec(form);
                    nextIsType = false;                
                    ste->setType(ts);
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
            //intermediate::type::FORM_STRINGS[ form_to_underlying(st->sortedEntries()[i]->getType()->getForm()) ];
            typestring = intermediate::type::FORM_STRINGS[form_to_underlying(st->sortedEntries()[i]->getType()->getForm())];
        }
        
        //form_to_underlying(st->sortedEntries()[i]->getType()->getForm());
        outsSt << i << " " << st->sortedEntries()[i]->getName() << "\t\t" << intermediate::symtab::KIND_STRINGS[kind_to_underlying(st->sortedEntries()[i]->getKind())] << "\t\t" << typestring << /*intermediate::type::FORM_STRINGS[form_to_underlying(st->sortedEntries()[i]->getType()->getForm())] << */endl;
    }
}

void generateSicXe(ifstream &insSymTab, ifstream &insLisp,
                 ofstream &outsSX) {
    /*
    for (int i = 0; i < st->sortedEntries().size(); i++) { //For each entry
        outsSX << i << " " << st->sortedEntries()[i]->getName() << "\t\t" << intermediate::symtab::KIND_STRINGS[kind_to_underlying(st->sortedEntries()[i]->getKind())] << "\t\t"  <<endl;
    }
    */
}

int main(int argc, const char *args[])
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
}
