#ifndef SCANNER_H_
#define SCANNER_H_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../Tokens/Token.h" 

using namespace std;

typedef enum {
    CHAR_LETTER,        //0
    CHAR_DIGIT,         //1
    CHAR_SINGLEQUOTE,   //2
    CHAR_SEMICOLON,     //3
    CHAR_DECIMAL,       //4
    CHAR_SPECIAL_SYM,   //5
    CHAR_WHITESPACE,    //6
    CHAR_EOF,           //7
    CHAR_UNKNOWN        //8
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

class Scanner
{
public:
    Scanner();
        Token read();
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

std::string nextToken(Scanner sc, ifstream& testFile, ofstream& out);

#endif /* SCANNER_H_ */