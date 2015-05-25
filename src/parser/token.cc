#include "token.h"

/// Constructor for the Token class
Token::Token( const TokenType type, const std::string value,
              const unsigned int startLineNo, const unsigned int startCharNo)
    : type(type),
      value(value),
      startLineNo(startLineNo),
      startCharNo(startCharNo) {
}

/// Returns token string
std::string Token::getValue() {
  return value;
}

/// Returns token type
TokenType Token::getType() {
  return type;
}

/// Returns the line number of the first character of the token
unsigned int Token::getStartLineNo() {
  return startLineNo;
}

/// Returns the character number of the first character of the token
unsigned int Token::getStartCharNo() {
  return startCharNo;
}
