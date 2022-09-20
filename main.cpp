//
//  main.cpp
//  Assignment-2
//
//  Created by Team 3 on 9/7/22.
//

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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

typedef enum {
    CHAR_LETTER,        //0
    CHAR_DIGIT,         //1
    CHAR_SINGLEQUOTE,   //2
    CHAR_DECIMAL,       //3
    CHAR_SPECIAL_SYM,   //4
    CHAR_WHITESPACE,    //5
    CHAR_EOF,           //6
    CHAR_UNKNOWN        //7
} char_state;

typedef enum {
    ST_FIRSTCHAR,     //0
    ST_WORD,          //1
    ST_OPERATOR,      //2
    ST_STRING,        //3
    ST_INTEGER,       //4
    ST_REAL_NUM,      //5
    ST_ERROR,         //6
    ST_END            //7
} token_state;

const std::string STRING_TOKEN = "string";
const std::string IDENTIFIER_TOKEN = "(identifier)";
const std::string INTEGER_TOKEN = "(integer)";
const std::string REAL_NUM_TOKEN = "(real number)";

int line_no = 1;
char c;
char_state cstate, p_cstate;
token_state tstate = ST_FIRSTCHAR;
std::string tok;

string nextToken(Scanner sc, ifstream& testFile, ofstream& out) {
    c = testFile.get();
    if (c == EOF) {
        cstate = CHAR_EOF;
    }
    else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        if (c == '\n') {
            line_no++;
        }
        cstate = CHAR_WHITESPACE;
    }
    else if (isalpha(c)) {
        cstate = CHAR_LETTER;
    }
    else if (isdigit(c)) {
        cstate = CHAR_DIGIT;
    }
    else if (c == '\'') {
        cstate = CHAR_SINGLEQUOTE;
    }
    else if (c == '.') {
        cstate = CHAR_DECIMAL;
    }
    else {
        cstate = CHAR_SPECIAL_SYM;
    }
    switch (tstate) {
    case ST_FIRSTCHAR:
        tok = ""; //Reset token
        if (cstate == CHAR_LETTER) {
            tstate = ST_WORD;
        }
        else if (cstate == CHAR_SPECIAL_SYM) {
            tstate = ST_OPERATOR;
        }
        else if (cstate == CHAR_SINGLEQUOTE) {
            tstate = ST_STRING;
        }
        else if (cstate == CHAR_DIGIT) {
            tstate = ST_INTEGER;
        }
        else if (cstate == CHAR_EOF) {
            tstate = ST_END;
        }
        else if (cstate == CHAR_WHITESPACE) {
            tstate = ST_FIRSTCHAR;  //Loop back
        }
        else { //Unexpected, go to error
            tstate = ST_ERROR;
        }
        break;
    case ST_WORD:
        if (cstate == CHAR_LETTER || cstate == CHAR_DIGIT) {  //Keep parsing, maintain state
            tstate = ST_WORD;
        }
        else if (cstate == CHAR_WHITESPACE) {   //End token
            std::string got_label = sc.GetLabel(tok); //Look up label with token
            //If token not in lookup table
            if (got_label == "") {  //Then it means it's an identifier
                got_label = sc.GetLabel(IDENTIFIER_TOKEN); //Look up label with identifier token
            }
            out << got_label << " : " << tok << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
            //Else, go to error
        }
        else { //Unexpected, go to error
            tstate = ST_ERROR;
        }
        break;
    case ST_OPERATOR:
        if (cstate == CHAR_SPECIAL_SYM) {   //Keep parsing, maintain state
            tstate = ST_OPERATOR;
        }
        else if (cstate == CHAR_WHITESPACE) { //End token
            std::string got_label = sc.GetLabel(tok); //Look up label with token
            //If token not in lookup table
            if (got_label == "") {  //Then error (because no return)
                tstate = ST_ERROR;
            }
            else {
                out << got_label << " : " << tok << endl;
                tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
            }
            //Else, go to error
        }
        else { //Unexpected, go to error
            tstate = ST_ERROR;
        }
        break;
    case ST_STRING:
        if (p_cstate == CHAR_SINGLEQUOTE && cstate == CHAR_WHITESPACE) { //End token
            std::string got_label = sc.GetLabel(STRING_TOKEN); //Look up label with string token
            out << got_label << " : " << tok << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
            //Else, go to error
        }
        break;
    case ST_INTEGER:
        if (cstate == CHAR_DIGIT) {   //Keep parsing, maintain state
            tstate = ST_INTEGER;
        }
        else if (cstate == CHAR_DECIMAL) {
            tstate = ST_REAL_NUM;
        }
        else if (cstate == CHAR_WHITESPACE) { //End token
            std::string got_label = sc.GetLabel(INTEGER_TOKEN); //Look up label with integer token
            out << got_label << " : " << tok << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
        }
        else { //Unexpected, go to error
            tstate = ST_ERROR;
        }
        break;
    case ST_REAL_NUM:
        if (cstate == CHAR_DIGIT) {   //Keep parsing, maintain state
            tstate = ST_REAL_NUM;
        }
        else if (cstate == CHAR_WHITESPACE) { //End token
            std::string got_label = sc.GetLabel(REAL_NUM_TOKEN); //Look up label with real number token
            out << got_label << " : " << tok << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
        }
        else { //Unexpected, go to error
            tstate = ST_ERROR;
        }
        break;
    case ST_ERROR:
        if (p_cstate == CHAR_WHITESPACE || cstate == CHAR_WHITESPACE) {
            out << "TOKEN ERROR at line " << line_no << ": \'" << tok << "\'" << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
        }
        break;
    case ST_END:
        out << "end" << endl;
        return "-1"; //EOF
    }
    if (tstate == ST_STRING || cstate != CHAR_WHITESPACE) {
        tok += c;
    }
    p_cstate = cstate;
    return tok;
}

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
    {"(identifier)",    "INDENTIFIER"}, //Placeholder Should be IDENTIFIER but assignment says INDENTIFIER???
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
    {";",               "SEMICOLOR"},   //Should be SEMICOLON but assignment says SEMICOLOR???
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
    unsigned int table_len = sizeof(table) / sizeof(table[0]);
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
    unsigned int table_len = sizeof(table) / sizeof(table[0]);
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

int readfile() {
    istringstream iss;
    std::ifstream myfile;
    string word;
    myfile.open("masterTestCase.txt");  //reads from masterTestCase
    ofstream output("masterTestCaseOut-1.txt"); //Outputs to MasterTestCaseOut-1
    std::string myline;
    string out;
    Scanner sc;

    if (myfile.is_open()) {
        while (!myfile.eof()) {
            std::getline(myfile, myline);
            if (myfile.good()) {    //reads word by word from the line
                iss.clear();    //clear out state
                iss.str(myline);
                while (iss.good()) {
                    iss >> word;
                    //word = nextToken(sc, word);
                    output << word << endl;
                    //output << word << endl; //output to different file
                } //while (word != "-1");
            }
        }
    }

    else {
        std::cout << "Couldn't open file\n";
    }

    return 0;
}

int main(int argc, const char* argv[]) {
    Scanner sc;
    string word;

    std::string inTestFileName = "test-in.txt";
    std::string outTestFileName = "test-out.txt";
    //std::string inTestFileName = "test-in-original.txt";
    //std::string outTestFileName = "test-out-original.txt";

    ifstream testFile;
    testFile.open(inTestFileName);
    
    ofstream out;
    out.open(outTestFileName);

    if (!testFile.is_open()) {
        cout << "Error while opening masterTestCase.txt";
    }
    else {
        while (word != "-1") {
            word = nextToken(sc, testFile, out);
            //cout << "[" << word << "]"; //link with hash table symbol table for output

        } 
    }

    testFile.close();
    cout << "Program complete, check: " << outTestFileName << endl;
    return 0;
}
