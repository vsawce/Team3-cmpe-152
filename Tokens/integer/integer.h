#ifndef INTEGER_H
#define INTEGER_H

#include <string>
#include "../index.h"

class IntegerToken : public Token
{
    public:
        static bool is_integer(char character);
        IntegerToken(std::string value, int line_number);
};

#endif