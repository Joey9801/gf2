#include <catch.h>

#include "../../src/parser/evaluator.h"

/// Evaluate an empty lexeme vector and check that it doesn't return any
/// errors and also doesn't produce any tokens
SCENARIO("Evaluate an empty lexeme vector") {
  Evaluator evaluator;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  evaluator.evaluate(lexemes, errors, tokens);

  CHECK(errors.empty());
  CHECK(tokens.empty());
} 

/// Evaluate a lexeme vector which contains only one lexeme - a single
/// identifier - and check that no errors are returned and one token is
/// produced
SCENARIO("Evaluate a lexeme vector which contains only one IDENTIFIER lexeme") {
  Evaluator evaluator;
  
  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "identifier", 1, 1));

  evaluator.evaluate(lexemes, errors, tokens);

  CHECK(errors.empty());
  REQUIRE(tokens.size() == 1);
  CHECK(tokens[0].getType() == TokenType::VALUE);
  CHECK(tokens[0].getValue() == "identifier");
  CHECK(tokens[0].getStartLineNo() == 1);
  CHECK(tokens[0].getStartCharNo() == 1);
}

/// Evaluate a lexeme vector which contains two lexemes - consecutive
/// identifiers - and check that the expected error is returned with no tokens
SCENARIO("Evaluate a lexeme vector which contains consecutive IDENTIFIER lexemes") {
  Evaluator evaluator;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "1stIdentifier", 1, 1));
  lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "2ndIdentifier", 1, 14));

  evaluator.evaluate(lexemes, errors, tokens);

  REQUIRE(errors.size() == 1);
  CHECK(errors[0].getErrorDescription() ==
      "EVALUATOR ERROR: Two consecutive IDENTIFIER lexemes received - aborting evaluation");
  CHECK(tokens.empty());
}
