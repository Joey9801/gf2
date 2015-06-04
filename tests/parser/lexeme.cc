#include <limits>
#include <catch.h>

#include "../../src/parser/lexeme.h"

SCENARIO("Create some lexemes and check their values") {
  /// Check that an identifier lexeme works as expected
  WHEN ("Create an identifier lexeme") {
    Lexeme lexeme(LexemeType::IDENTIFIER, "identifier", 13, 12);
    REQUIRE(lexeme.getType() == LexemeType::IDENTIFIER);
    REQUIRE(lexeme.getString() == "identifier");
    REQUIRE(lexeme.getStartLineNo() == (unsigned long long int)13);
    REQUIRE(lexeme.getStartCharNo() == (unsigned long long int)12);
  }

  /// Check that a string lexeme works as expected
  WHEN ("Create a string lexeme") {
    Lexeme lexeme(LexemeType::STRING, "this is a string", 100, 1);
    REQUIRE(lexeme.getType() == LexemeType::STRING);
    REQUIRE(lexeme.getString() == "this is a string");
    REQUIRE(lexeme.getStartLineNo() == (unsigned long long int)100);
    REQUIRE(lexeme.getStartCharNo() == (unsigned long long int)1);
  }

  /// Check that a singularity lexeme works as expected
  WHEN ("Create a singularity lexeme") {
    Lexeme lexeme(LexemeType::SINGULARITY, ":", 66543, 456);
    REQUIRE(lexeme.getType() == LexemeType::SINGULARITY);
    REQUIRE(lexeme.getString() == ":");
    REQUIRE(lexeme.getStartLineNo() == (unsigned long long int)66543);
    REQUIRE(lexeme.getStartCharNo() == (unsigned long long int)456);
  }

  /// Check that a whitespace lexeme works as expected
  /// Check maximum value boundary case for positions
  WHEN("Create a whitespace lexeme") {
    Lexeme lexeme(LexemeType::WHITESPACE, "         ",
                  std::numeric_limits<unsigned int>::max(),
                  std::numeric_limits<unsigned int>::max());
    REQUIRE(lexeme.getType() == LexemeType::WHITESPACE);
    REQUIRE(lexeme.getString() == "         ");
    REQUIRE(lexeme.getStartLineNo() ==
      std::numeric_limits<unsigned int>::max());
    REQUIRE(lexeme.getStartCharNo() ==
      std::numeric_limits<unsigned int>::max());
  }

  /// Check that a comment lexeme works as expected
  /// Check minimum value boundary case for positions
  WHEN ("Create a comment lexeme") {
    Lexeme lexeme(LexemeType::COMMENT, "/* This is a comment */",
      std::numeric_limits<unsigned int>::min(),
      std::numeric_limits<unsigned int>::min());
    REQUIRE(lexeme.getType() == LexemeType::COMMENT);
    REQUIRE(lexeme.getString() == "/* This is a comment */");
    REQUIRE(lexeme.getStartLineNo() ==
      std::numeric_limits<unsigned int>::min());
    REQUIRE(lexeme.getStartCharNo() ==
      std::numeric_limits<unsigned int>::min());
  }
}
