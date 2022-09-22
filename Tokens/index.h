#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <fstream>
#include "../Tokens/tokenType/tokenType.h"

class Token
{
    public:
        Token();
        Token(TokenTypeId typeId, std::string value, int line_number);
        Token(TokenTypeId typeId, std::string value);
        Token(TokenTypeId typeId, char value);
        bool is_eof();
        bool is_identifier();
        bool is_integer();
        std::string get_value();
        
    private:
        TokenTypeId typeId;
        std::string value;
        int line_number;
        friend std::ostream& operator<<(std::ostream&, const Token&);
        friend bool operator==(const Token&, const Token&);
        friend bool operator!=(const Token&, const Token&);
        friend bool operator<(const Token&, const Token&);
};

#endif