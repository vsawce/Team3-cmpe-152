#include <stdio.h>
#include "eof.h"

 bool EndOfFileToken::is_eof(char character)
 {
     return character == EOF;
 }

EndOfFileToken::EndOfFileToken(int line_number)
: Token(EndOfFileTokenId, "EOF", line_number) {}