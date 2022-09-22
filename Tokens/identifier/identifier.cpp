#include "identifier.h"

IdentifierToken::IdentifierToken(std::string value, int line_number)
: Token(IdentifierTokenId, value, line_number) {
}