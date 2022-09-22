#include "../inc/scanner.h"

using namespace std;

const std::string STRING_TOKEN = "string";
const std::string IDENTIFIER_TOKEN = "(identifier)";
const std::string INTEGER_TOKEN = "(integer)";
const std::string REAL_NUM_TOKEN = "(real number)";

int line_no = 1;
bool first_quote = true;
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
    else if (c == ';') {
        cstate = CHAR_SEMICOLON;
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
        else if (cstate == CHAR_SPECIAL_SYM || cstate == CHAR_SEMICOLON || cstate == CHAR_DECIMAL) {
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
        cout << "wordtok\t" << tok << "\tC=" << c << "\tcstate=" << cstate << endl;
        if (cstate == CHAR_LETTER || cstate == CHAR_DIGIT) {  //Keep parsing, maintain state
            tstate = ST_WORD;
        }
        else if (cstate == CHAR_WHITESPACE || cstate == CHAR_SEMICOLON || cstate == CHAR_SPECIAL_SYM
                || cstate == CHAR_DECIMAL) {   //End token
            std::string got_label = sc.GetLabel(tok); //Look up label with token
            //If token not in lookup table
            if (got_label == "") {  //Then it means it's an identifier
                got_label = sc.GetLabel(IDENTIFIER_TOKEN); //Look up label with identifier token
            }
            out << got_label << " : " << tok << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
            if (cstate == CHAR_SEMICOLON || cstate == CHAR_SPECIAL_SYM || cstate == CHAR_SINGLEQUOTE || cstate == CHAR_DECIMAL) {
                got_label = sc.GetLabel(std::string(1, c));
                out << got_label << " : "<< std::string(1, c) << endl;
            }
            //Else, go to error
        }
        else { //Unexpected, go to error
            tstate = ST_ERROR;
        }
        break;
    case ST_OPERATOR:
        cout << "optok\t" << tok << "\tC=" << c << "\tcstate=" << cstate << endl;
        if (cstate == CHAR_SPECIAL_SYM) {   //Keep parsing, maintain state. Only check special sym since other important special chars are 1 wide
            tstate = ST_OPERATOR;
        }
        else if (cstate == CHAR_WHITESPACE || cstate == CHAR_SEMICOLON || cstate == CHAR_SPECIAL_SYM
                || cstate == CHAR_SINGLEQUOTE) { //End token
            std::string got_label = sc.GetLabel(tok); //Look up label with token
            //If token not in lookup table
            if (cstate == CHAR_SINGLEQUOTE) {
                cout << "str" << endl;
                tstate = ST_STRING;
            }
            else if (got_label == "") {  //Then error (because no return)
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
        cout << "strtok\t" << tok << "\tC=" << c << "\tcstate=" << cstate << endl;
        if (p_cstate == CHAR_SINGLEQUOTE && first_quote) {
            first_quote = false;
        }
        else if (p_cstate == CHAR_SINGLEQUOTE && (cstate == CHAR_WHITESPACE || cstate == CHAR_SPECIAL_SYM)) { //End token
            std::string got_label = sc.GetLabel(STRING_TOKEN); //Look up label with string token
            cout << got_label << " : " << tok << endl;
            out << got_label << " : " << tok << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
            //Else, go to error
            first_quote = true;
            if (cstate == CHAR_SPECIAL_SYM) {
                got_label = sc.GetLabel(std::string(1, c));
                out << got_label << " : "<< std::string(1, c) << endl;
            }
        }
        break;
    case ST_INTEGER:
        if (cstate == CHAR_DIGIT) {   //Keep parsing, maintain state
            tstate = ST_INTEGER;
        }
        else if (cstate == CHAR_DECIMAL) {
            tstate = ST_REAL_NUM;
        }
        else if (cstate == CHAR_WHITESPACE || cstate == CHAR_SEMICOLON) { //End token
            std::string got_label = sc.GetLabel(INTEGER_TOKEN); //Look up label with integer token
            out << got_label << " : " << tok << endl;
            tstate = ST_FIRSTCHAR; //Go back to assuming next character is first character of next token
            if (cstate == CHAR_SEMICOLON || cstate == CHAR_SPECIAL_SYM) {
                got_label = sc.GetLabel(std::string(1, c));
                out << got_label << " : "<< std::string(1, c) << endl;
            }
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
    {";",               "SEMICOLON"},   //Should be SEMICOLON but assignment says SEMICOLOR???
    {",",               "COMMA"},
    {"(",               "LPAREN"},
    {")",               "RPAREN"},
    {"[",               "LBRACKET"},
    {"]",               "RBRACKET"},
    {"{",               "LBRACE"},
    {"}",               "RBRACE"},
    {"(*",              "LCOMMENT"},
    {"*)",              "RCOMMENT"},
    {".",               "PERIOD"}     //Added for compatibility
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