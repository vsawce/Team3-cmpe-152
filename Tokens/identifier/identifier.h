#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <string>
#include "../index.h"

class IdentifierToken : public Token
{
    public:
        static bool is_identifier(Token token);
        IdentifierToken(std::string value, int line_number);
};

#endif