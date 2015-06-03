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
    CHECK(errors[0].getLineNo() == (unsigned long long int)0);
    CHECK(errors[0].getCharNo() == (unsigned long long int)0);

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
  CHECK(tokens[ 0].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 0].getStartCharNo() == (unsigned long long int)1);

  CHECK(tokens[ 1].getType() == TokenType::VALUE);
  CHECK(tokens[ 1].getValue() == "identifier.lexemes");
  CHECK(tokens[ 1].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 1].getStartCharNo() == (unsigned long long int)7);

  CHECK(tokens[ 2].getType() == TokenType::DICTDELIMOPEN);
  CHECK(tokens[ 2].getValue() == "{");
  CHECK(tokens[ 2].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 2].getStartCharNo() == (unsigned long long int)26);

  CHECK(tokens[ 3].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[ 3].getValue() == "should");
  CHECK(tokens[ 3].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 3].getStartCharNo() == (unsigned long long int)27);

  CHECK(tokens[ 4].getType() == TokenType::DICTDELIMCLOSE);
  CHECK(tokens[ 4].getValue() == "}");
  CHECK(tokens[ 4].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 4].getStartCharNo() == (unsigned long long int)33);

  CHECK(tokens[ 5].getType() == TokenType::STRING);
  CHECK(tokens[ 5].getValue() == "make it through");
  CHECK(tokens[ 5].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 5].getStartCharNo() == (unsigned long long int)35);

  CHECK(tokens[ 6].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[ 6].getValue() == "to");
  CHECK(tokens[ 6].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 6].getStartCharNo() == (unsigned long long int)53);

  CHECK(tokens[ 7].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[ 7].getValue() == "the");
  CHECK(tokens[ 7].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 7].getStartCharNo() == (unsigned long long int)56);

  CHECK(tokens[ 8].getType() == TokenType::STRING);
  CHECK(tokens[ 8].getValue() == "evaluator");
  CHECK(tokens[ 8].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 8].getStartCharNo() == (unsigned long long int)59);

  CHECK(tokens[ 9].getType() == TokenType::VALUE);
  CHECK(tokens[ 9].getValue() == "class.successfully");
  CHECK(tokens[ 9].getStartLineNo() == (unsigned long long int)5);
  CHECK(tokens[ 9].getStartCharNo() == (unsigned long long int)71);

  CHECK(tokens[10].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[10].getValue() == "identifier");
  CHECK(tokens[10].getStartLineNo() == (unsigned long long int)9);
  CHECK(tokens[10].getStartCharNo() == (unsigned long long int)1);

  CHECK(tokens[11].getType() == TokenType::DICTDELIMOPEN);
  CHECK(tokens[11].getValue() == "{");
  CHECK(tokens[11].getStartLineNo() == (unsigned long long int)10);
  CHECK(tokens[11].getStartCharNo() == (unsigned long long int)1);

  CHECK(tokens[12].getType() == TokenType::STRING);
  CHECK(tokens[12].getValue() == "string after singularity test");
  CHECK(tokens[12].getStartLineNo() == (unsigned long long int)10);
  CHECK(tokens[12].getStartCharNo() == (unsigned long long int)2);

  CHECK(tokens[13].getType() == TokenType::DICTDELIMCLOSE);
  CHECK(tokens[13].getValue() == "}");
  CHECK(tokens[13].getStartLineNo() == (unsigned long long int)10);
  CHECK(tokens[13].getStartCharNo() == (unsigned long long int)33);

  CHECK(tokens[14].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[14].getValue() == "final");
  CHECK(tokens[14].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[14].getStartCharNo() == (unsigned long long int)1);

  CHECK(tokens[15].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[15].getValue() == "test");
  CHECK(tokens[15].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[15].getStartCharNo() == (unsigned long long int)7);

  CHECK(tokens[16].getType() == TokenType::PAIRSEPARATOR);
  CHECK(tokens[16].getValue() == ":");
  CHECK(tokens[16].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[16].getStartCharNo() == (unsigned long long int)11);

  CHECK(tokens[17].getType() == TokenType::DICTSEPARATOR);
  CHECK(tokens[17].getValue() == ",");
  CHECK(tokens[17].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[17].getStartCharNo() == (unsigned long long int)12);

  CHECK(tokens[18].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[18].getValue() == "singularity");
  CHECK(tokens[18].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[18].getStartCharNo() == (unsigned long long int)13);

  CHECK(tokens[19].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[19].getValue() == "followed");
  CHECK(tokens[19].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[19].getStartCharNo() == (unsigned long long int)25);

  CHECK(tokens[20].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[20].getValue() == "by");
  CHECK(tokens[20].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[20].getStartCharNo() == (unsigned long long int)34);

  CHECK(tokens[21].getType() == TokenType::IDENTIFIER);
  CHECK(tokens[21].getValue() == "singularity");
  CHECK(tokens[21].getStartLineNo() == (unsigned long long int)11);
  CHECK(tokens[21].getStartCharNo() == (unsigned long long int)37);
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
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)58);

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
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)75);

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
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)72);

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
      CHECK(errors[0].getLineNo() == (unsigned long long int)2);
      CHECK(errors[0].getCharNo() == (unsigned long long int)1);

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
      CHECK(errors[0].getLineNo() == (unsigned long long int)2);
      CHECK(errors[0].getCharNo() == (unsigned long long int)1);

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
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)1);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing a forward slash followed by an identifier and check
  /// that it returns an evaluator error and no tokens
  GIVEN("Lex file containing a forward slash followed immediately by an identifier") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_identifier.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)6);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing a forward slash followed by a valid singularity
  /// (delimiter/separator) and check that it returns an evaluator error and
  /// no tokens
  GIVEN("Lex file containing a forward slash followed immediately by a valid singularity") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_singularity.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)6);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing a forward slash followed by a string and check that
  /// it returns an evaluator error and no tokens
  GIVEN("Lex file containing a forward slash followed immediately by a string") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_string.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)6);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file containing a forward slash followed by whitespace and check that
  /// it returns an evaluator error and no tokens
  GIVEN("Lex file containing a forward slash followed immediately by whitespace") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/forward_slash_whitespace.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
        "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)6);

      CHECK(tokens.empty());
    }
  }
}

/// Lex files which will cover all evaluator errors
SCENARIO("Lex files which will cover all evaluator errors") {
  Lexor lexor;

  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  /// Lex a file which results in an invalid singularity lexeme transition out
  /// of the IDLE state of the evaluator FSM
  GIVEN("Lex file containing an invalid singularity lexeme after whitespace") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/invalid_singularity_from_idle.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
        "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)30);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file which results in an invalid singularity lexeme transition out
  /// of the SINGLEIDENTIFIER state of the evaluator FSM
  GIVEN("Lex file containing an invalid singularity lexeme after an identifier") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/invalid_singularity_from_identifier.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
        "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter, separator, member access operator or identifier - aborting evaluation");
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)11);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file which results in an invalid singularity lexeme transition out
  /// of the MEMBERACCESSEDIDENTIFIER state of the evaluator FSM
  GIVEN("Lex file containing an invalid singularity lexeme after a member access operator") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/invalid_singularity_from_member_access.def",
              errors,
              tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
        "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
      CHECK(errors[0].getLineNo() == (unsigned long long int)1);
      CHECK(errors[0].getCharNo() == (unsigned long long int)12);

      CHECK(tokens.empty());
    }
  }

  /// Lex a file which ends immediately after a member access operator
  GIVEN("Lex file which ends after a member access operator") {
    lexor.lex("tests/parser/test_files/lexor_tests/evaluator_failures/end_from_member_access.def",
      errors,
      tokens);

    THEN("An error and no tokens are produced") {
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
        "EVALUATOR ERROR: Premature end of file - expected identifier after member access operator");
      CHECK(errors[0].getLineNo() == (unsigned long long int)0);
      CHECK(errors[0].getCharNo() == (unsigned long long int)0);

      CHECK(tokens.empty());
    }
  }
}
