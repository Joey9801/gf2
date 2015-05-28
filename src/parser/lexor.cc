#include "lexor.h"

#include "lexeme.h"

/// Constructor for the Lexor class
Lexor::Lexor() : scanner(), evaluator() {
}

/// Lexes file and returns a vector of the tokens it contains
bool Lexor::lex(const std::string filename,
                std::vector<ParserError>& errors,
                std::vector<Token>& tokens) {
  std::vector<Lexeme> lexemes;
  // If the scanner gives errors, do not attempt to evaluate the lexeme vector
  // and return false to indicate failure
  if (scanner.scan(filename, errors, lexemes)) {
    return evaluator.evaluate(lexemes, errors, tokens);
  }
  return false;
}
