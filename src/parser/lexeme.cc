#include "lexeme.h"

/// Constructor for the Lexeme class
Lexeme::Lexeme( const LexemeType type, const std::string charString,
                const unsigned long long int startLineNo,
                const unsigned long long int startCharNo)
    : type(type),
      charString(charString),
      startLineNo(startLineNo),
      startCharNo(startCharNo) {
}

/// Returns lexeme string
std::string Lexeme::getString() {
  return charString;
}

/// Returns lexeme type
LexemeType Lexeme::getType() {
  return type;
}

/// Returns the line number of the first character of the lexeme
unsigned long long int Lexeme::getStartLineNo() {
  return startLineNo;
}

/// Returns the character number of the first character of the lexeme
unsigned long long int Lexeme::getStartCharNo() {
  return startCharNo;
}
