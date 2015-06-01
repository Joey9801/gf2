#include <catch.h>

#include "../../src/parser/lexor.h"

/// Try to lex a non-existent file and check that it returns a scanner error
/// and no tokens
SCENARIO("Try to lex a non-existent file") {
  Lexor lexor;

  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  lexor.lex("tests/parser/test_files/lexor_tests/non_existent_file.def",
            errors,
            tokens);

  THEN("An error and no tokens are produced") {
    REQUIRE(errors.size() == 1);
    CHECK(errors[0].getErrorDescription() ==
        "SCANNER ERROR: File open operation failed - aborted scanning");
    CHECK(errors[0].getLineNo() == 0);
    CHECK(errors[0].getCharNo() == 0);

    CHECK(tokens.empty());
  }
}

/// Lex an empty file and check that it returns no errors and no tokens
SCENARIO("Lex an empty file") {
  Lexor lexor;

  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  lexor.lex("tests/parser/test_files/lexor_tests/blank_file.def",
            errors,
            tokens);

  THEN("No errors and no tokens are produced") {
    CHECK(errors.empty());
    CHECK(tokens.empty());
  }
}

/// Lex a file which will test all transitions of both the scanner and
/// evaluator FSMs which can be covered without generating any errors
/// in either
SCENARIO("Lex file covering all errorless transitions") {
  Lexor lexor;

  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  lexor.lex("tests/parser/test_files/lexor_tests/errorless_transitions.def",
    errors,
    tokens);

  CHECK(errors.empty());
  REQUIRE(tokens.size() == 22);

  CHECK(tokens[ 0].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[ 0].getValue() == "These");
  CHECK(tokens[ 0].getStartLineNo() == 5);
  CHECK(tokens[ 0].getStartCharNo() == 1);

  CHECK(tokens[ 1].getType() == TokenType::VALUE);
  CHECK(tokens[ 1].getValue() == "identifier.lexemes");
  CHECK(tokens[ 1].getStartLineNo() == 5);
  CHECK(tokens[ 1].getStartCharNo() == 7);

  CHECK(tokens[ 2].getType() == TokenType::DICTDELIMOPEN);
  CHECK(tokens[ 2].getValue() == "{");
  CHECK(tokens[ 2].getStartLineNo() == 5);
  CHECK(tokens[ 2].getStartCharNo() == 26);

  CHECK(tokens[ 3].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[ 3].getValue() == "should");
  CHECK(tokens[ 3].getStartLineNo() == 5);
  CHECK(tokens[ 3].getStartCharNo() == 27);

  CHECK(tokens[ 4].getType() == TokenType::DICTDELIMCLOSE);
  CHECK(tokens[ 4].getValue() == "}");
  CHECK(tokens[ 4].getStartLineNo() == 5);
  CHECK(tokens[ 4].getStartCharNo() == 33);

  CHECK(tokens[ 5].getType() == TokenType::STRING);
  CHECK(tokens[ 5].getValue() == "make it through");
  CHECK(tokens[ 5].getStartLineNo() == 5);
  CHECK(tokens[ 5].getStartCharNo() == 35);

  CHECK(tokens[ 6].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[ 6].getValue() == "to");
  CHECK(tokens[ 6].getStartLineNo() == 5);
  CHECK(tokens[ 6].getStartCharNo() == 53);

  CHECK(tokens[ 7].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[ 7].getValue() == "the");
  CHECK(tokens[ 7].getStartLineNo() == 5);
  CHECK(tokens[ 7].getStartCharNo() == 56);

  CHECK(tokens[ 8].getType() == TokenType::STRING);
  CHECK(tokens[ 8].getValue() == "evaluator");
  CHECK(tokens[ 8].getStartLineNo() == 5);
  CHECK(tokens[ 8].getStartCharNo() == 59);

  CHECK(tokens[ 9].getType() == TokenType::VALUE);
  CHECK(tokens[ 9].getValue() == "class.successfully");
  CHECK(tokens[ 9].getStartLineNo() == 5);
  CHECK(tokens[ 9].getStartCharNo() == 71);

  CHECK(tokens[10].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[10].getValue() == "identifier");
  CHECK(tokens[10].getStartLineNo() == 9);
  CHECK(tokens[10].getStartCharNo() == 1);

  CHECK(tokens[11].getType() == TokenType::DICTDELIMOPEN);
  CHECK(tokens[11].getValue() == "{");
  CHECK(tokens[11].getStartLineNo() == 10);
  CHECK(tokens[11].getStartCharNo() == 1);

  CHECK(tokens[12].getType() == TokenType::STRING);
  CHECK(tokens[12].getValue() == "string after singularity test");
  CHECK(tokens[12].getStartLineNo() == 10);
  CHECK(tokens[12].getStartCharNo() == 2);

  CHECK(tokens[13].getType() == TokenType::DICTDELIMCLOSE);
  CHECK(tokens[13].getValue() == "}");
  CHECK(tokens[13].getStartLineNo() == 10);
  CHECK(tokens[13].getStartCharNo() == 33);

  CHECK(tokens[14].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[14].getValue() == "final");
  CHECK(tokens[14].getStartLineNo() == 11);
  CHECK(tokens[14].getStartCharNo() == 1);

  CHECK(tokens[15].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[15].getValue() == "test");
  CHECK(tokens[15].getStartLineNo() == 11);
  CHECK(tokens[15].getStartCharNo() == 7);

  CHECK(tokens[16].getType() == TokenType::PAIRSEPARATOR);
  CHECK(tokens[16].getValue() == ":");
  CHECK(tokens[16].getStartLineNo() == 11);
  CHECK(tokens[16].getStartCharNo() == 11);

  CHECK(tokens[17].getType() == TokenType::DICTSEPARATOR);
  CHECK(tokens[17].getValue() == ",");
  CHECK(tokens[17].getStartLineNo() == 11);
  CHECK(tokens[17].getStartCharNo() == 12);

  CHECK(tokens[18].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[18].getValue() == "singularity");
  CHECK(tokens[18].getStartLineNo() == 11);
  CHECK(tokens[18].getStartCharNo() == 13);

  CHECK(tokens[19].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[19].getValue() == "followed");
  CHECK(tokens[19].getStartLineNo() == 11);
  CHECK(tokens[19].getStartCharNo() == 25);

  CHECK(tokens[20].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[20].getValue() == "by");
  CHECK(tokens[20].getStartLineNo() == 11);
  CHECK(tokens[20].getStartCharNo() == 34);

  CHECK(tokens[21].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[21].getValue() == "singularity");
  CHECK(tokens[21].getStartLineNo() == 11);
  CHECK(tokens[21].getStartCharNo() == 37);
}

/// Lex files to generate all possible scanner errors
SCENARIO("Lex files to generate all scanner errors") {
  Lexor lexor;

  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  /// Lex a file containing an invalid character and check that it returns
  /// a scanner error and no tokens
  GIVEN("Lex file containing an invalid character") {
    lexor.lex("tests/parser/test_files/lexor_tests/scanner_failures/invalid_char.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 58);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing an invalid character in a comment and check that
  /// it returns a scanner error and no tokens
  GIVEN("Lex file containing an invalid character in a comment") {
    lexor.lex("tests/parser/test_files/lexor_tests/scanner_failures/invalid_char_comment.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      // All characters which are valid in the file are valid in a comment, so the
      // error returned will be about the invalid character in the file, not in the
      // comment
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 75);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing an invalid character in a string and check that
  /// it returns a scanner error and no tokens
  GIVEN("Lex file containing an invalid character in a string") {
    lexor.lex("tests/parser/test_files/lexor_tests/scanner_failures/invalid_char_string.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      // All characters which are valid in the file are valid in a string, so the
      // error returned will be about the invalid character in the file, not in the
      // string
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 72);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing an unterminated comment and check that it returns a
  /// scanner error and no tokens
  GIVEN("Lex file containing an unterminated comment") {
    lexor.lex("tests/parser/test_files/lexor_tests/scanner_failures/unterm_comment.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Comment unterminated by end of file");
      CHECK(errors[0].getLineNo() == 2);
      CHECK(errors[0].getCharNo() == 1);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing an unterminated string and check that it returns a
  /// scanner error and no tokens
  GIVEN("Lex file containing an unterminated string") {
    lexor.lex("tests/parser/test_files/lexor_tests/scanner_failures/unterm_string.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: String unterminated by end of file");
      CHECK(errors[0].getLineNo() == 2);
      CHECK(errors[0].getCharNo() == 1);

      CHECK(tokens.empty());
    }
  }
}

/// Lex files which will cover all transitions of the scanner FSM which will
/// produce evaluator errors
SCENARIO("Lex files which cover scanner FSM transitions which produce evaluator errors") {
  Lexor lexor;

  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  /// Lex a file containing a forward slash followed by another and check that
  /// it returns an evaluator error and no tokens
  GIVEN("Lex file containing two consecutive forward slashes") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_forward_slash.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 1);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing a forward slash followed by an identifier and check
  /// that it returns an evaluator error and one token (for the preceeding
  /// identifier)
  GIVEN("Lex file containing a forward slash followed immediately by an identifier") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_identifier.def",
              errors,
              tokens);

    THEN("An error and a token are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 6);

      CHECK(tokens.size() == 1);
    }
  }

  /// Lex a file containing a forward slash followed by a valid singularity
  /// (delimiter/separator) and check that it returns an evaluator error and
  /// one token (for the preceeding identifier)
  GIVEN("Lex file containing a forward slash followed immediately by a valid singularity") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_singularity.def",
              errors,
              tokens);

    THEN("An error and a token are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 6);

      CHECK(tokens.size() == 1);
    }
  }

  /// Lex a file containing a forward slash followed by a string and check that
  /// it returns an evaluator error and one token (for the preceeding
  /// identifier)
  GIVEN("Lex file containing a forward slash followed immediately by a string") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_string.def",
              errors,
              tokens);

    THEN("An error and a token are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 6);

      CHECK(tokens.size() == 1);
    }
  }

  /// Lex a file containing a forward slash followed by whitespace and check that
  /// it returns an evaluator error and one token (for the preceeding identifier)
  GIVEN("Lex file containing a forward slash followed immediately by whitespace") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_whitespace.def",
              errors,
              tokens);

    THEN("An error and a token are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
        "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == 1);
      CHECK(errors[0].getCharNo() == 6);

      CHECK(tokens.size() == 1);
    }
  }
}
