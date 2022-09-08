//
//  main.cpp
//  Assignment2
//
//  Created by MSSV on 9/7/22.
//
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

class Scanner
{
public:
    Scanner();
    std::string GetLabel(std::string token) const;
    std::string GetToken(std::string label) const;
    void nexttoken()
    {
        return;
    }
private:
    ////////////////////
    // Column 0 - Token
    // Column 1 - Label
    ////////////////////
    std::string table[128][2];
};

//Assign token/labels in constructor
Scanner::Scanner()
: table
    {
        {"and",            "AND"},
        {"array",           "ARRAY"},
        {"asm",             "ASM"},
        {"begin",           "BEGIN"},
        {"break",           "BREAK"},
        {"case",            "CASE"},
        {"const",           "CONST"},
        {"constructor",     "CONSTRUCTOR"},
        {"continue",        "CONTINUE"},
        {"destructor",      "DESTRUCTOR"},
        {"div",             "DIV"},
        {"do",              "DO"},
        {"downto",          "DOWNTO"},
        {"else",            "ELSE"},
        {"end",             "END"},
        {"FALSE",           "FALSE"},
        {"file",            "FILE"},
        {"for",             "FOR"},
        {"function",        "FUNCTION"},
        {"goto",            "GOTO"},
        {"if",              "IF"},
        {"implementation",  "IMPLEMENTATION"},
        {"in",              "IN"},
        {"inline",          "INLINE"},
        {"interface",       "INTERFACE"},
        {"label",           "LABEL"},
        {"mod",             "MOD"},
        {"nil",             "NIL"},
        {"not",             "NOT"},
        {"object",          "OBJECT"},
        {"of",              "OF"},
        {"on",              "ON"},
        {"operator",        "OPERATOR"},
        {"or",              "OR"},
        {"packed",          "PACKED"},
        {"procedure",       "PROCEDURE"},
        {"program",         "PROGRAM"},
        {"record",          "RECORD"},
        {"repeat",          "REPEAT"},
        {"set",             "SET"},
        {"shl",             "SHL"},
        {"shr",             "SHR"},
        {"string",          "STRING"},
        {"then",            "THEN"},
        {"to",              "TO"},
        {"TRUE",            "TRUE"},
        {"type",            "TYPE"},
        {"unit",            "UNIT"},
        {"until",           "UNTIL"},
        {"uses",            "USES"},
        {"var",             "VAR"},
        {"while",           "WHILE"},
        {"with",            "WITH"},
        {"xor",             "XOR"},
        {"(integer)",       "INTEGER"},     //Placeholder
        {"(real number)",   "REAL"},        //Placeholder
        {"(identifier)",    "INDENTIFIER"}, //Placeholder
        {"+",               "PLUSOP"},
        {"-",               "MINUSOP"},
        {"*",               "MULTOP"},
        {"/",               "DIVOP"},
        {":=",              "ASSIGN"},
        {"=",               "EQUAL"},
        {"<>",              "NE"},
        {"<=",              "LTEQ"},
        {">=",              "GTEQ"},
        {"<",               "LT"},
        {">",               "GT"},
        {"+=",              "PLUSEQUAL"},
        {"-=",              "MINUSEQUAL"},
        {"*=",              "MULTEQUAL"},
        {"/=",              "DIVEQUAL"},
        {"^",               "CARAT"},
        {";",               "SEMICOLOR"},
        {",",               "COMMA"},
        {"(",               "LPAREN"},
        {")",               "RPAREN"},
        {"[",               "LBRACKET"},
        {"]",               "RBRACKET"},
        {"{",               "LBRACE"},
        {"}",               "RBRACE"},
        {"(*",              "LCOMMENT"},
        {"*)",              "RCOMMENT"}
    }
{

}

std::string Scanner::GetLabel(std::string token) const
{
    unsigned int table_len = sizeof(table)/sizeof(table[0]);
    //Convert token to lowercase
    //      Your scanner should not be case sensitive. In other words, it should recognize
    //      “array”, “ARRAY”, or even “ArRAy.”
    transform(token.begin(), token.end(), token.begin(), ::tolower);
    //Search through each row
    for (unsigned int i = 0; i < table_len; i++) {
        if (table[i][0] == token)
            return table[i][1];
    }
    //Return empty string if not found
    return "";
}

std::string Scanner::GetToken(std::string label) const
{
    unsigned int table_len = sizeof(table)/sizeof(table[0]);
    //Convert label to uppercase
    transform(label.begin(), label.end(), label.begin(), ::toupper);
    //Search through each row
    for (unsigned int i = 0; i < table_len; i++) {
        if (table[i][1] == label)
            return table[i][0];
    }
    //Return empty string if not found
    return "";
}

int main(int argc, const char * argv[]) {
    Scanner sc;
    cout << "Hello, World!" << endl;

    //Accessor test code
    cout << sc.GetLabel("for") << endl;
    cout << sc.GetLabel("fOr") << endl;
    cout << sc.GetLabel("FOR") << endl;
    cout << sc.GetLabel("(*") << endl;
    cout << sc.GetLabel("bingbong") << endl;
    return 0;
}
