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

/// Evaluate lexeme vectors which ensure coverage of all transitions
/// out of all states of the Evaluator class finite state machine
/// and check they produce the expected output
SCENARIO("Evaluate all transitions out of all states") {
  Evaluator evaluator;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  std::vector<Token> tokens;

  GIVEN("In idle state") {
    // Already in the IDLE state

    // Set up line and char numbers for next lexeme insertion
    const unsigned int lineNo = 1;
    const unsigned int charNo = 1;

    // Transition out of state with IDENTIFIER lexeme
    WHEN("Transition out of state with identifier lexeme") {
      lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "identifier", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors and identifier tokenised as value") {
        // Expect identifier to be tokenised as a VALUE token and no errors
        // to be produced
        CHECK(errors.empty());
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].getType() == TokenType::VALUE);
        CHECK(tokens[0].getValue() == "identifier");
        CHECK(tokens[0].getStartLineNo() == lineNo);
        CHECK(tokens[0].getStartCharNo() == charNo);
      }
    }

    // Transition out of state with WHITESPACE lexeme
    WHEN("Transition out of state with whitespace lexeme") {
      lexemes.push_back(Lexeme(LexemeType::WHITESPACE, " ", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors and no tokens") {
        // Expect no errors and no tokens
        CHECK(errors.empty());
        CHECK(tokens.empty());
      }
    }

    // Transition out of state with COMMENT lexeme
    WHEN("Transition out of state with comment lexeme") {
      lexemes.push_back(Lexeme(LexemeType::COMMENT, "/*comment*/", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors and no tokens") {
        // Expect no errors and no tokens
        CHECK(errors.empty());
        CHECK(tokens.empty());
      }
    }

    // Transition out of state with STRING lexeme
    WHEN("Transition out of state with string lexeme") {
      lexemes.push_back(Lexeme(LexemeType::STRING, "a string", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors and string tokenised as string") {
        // Expect string to be tokenised as a STRING token and no errors
        // to be produced
        CHECK(errors.empty());
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].getType() == TokenType::STRING);
        CHECK(tokens[0].getValue() == "a string");
        CHECK(tokens[0].getStartLineNo() == lineNo);
        CHECK(tokens[0].getStartCharNo() == charNo);
      }
    }

    // Transition out of state with SINGULARITY lexeme
    WHEN("Transition out of state with singularity lexeme") {
      // Transition out of state with opening dict delimiter
      WHEN("Singularity is an opening dict delimiter") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "{", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors and singularity tokenised as opening dict delimiter") {
          // Expect singularity to be tokenised as DICTDELIMOPEN and no errors
          // to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 1);
          CHECK(tokens[0].getType() == TokenType::DICTDELIMOPEN);
          CHECK(tokens[0].getValue() == "{");
          CHECK(tokens[0].getStartLineNo() == lineNo);
          CHECK(tokens[0].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with closing dict delimiter
      WHEN("Singularity is a closing dict delimiter") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "}", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors and singularity tokenised as closing dict delimiter") {
          // Expect singularity to be tokenised as DICTDELIMCLOSE and no errors
          // to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 1);
          CHECK(tokens[0].getType() == TokenType::DICTDELIMCLOSE);
          CHECK(tokens[0].getValue() == "}");
          CHECK(tokens[0].getStartLineNo() == lineNo);
          CHECK(tokens[0].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with dict separator
      WHEN("Singularity is a dict separator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ",", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors and singularity tokenised as dict separator") {
          // Expect singularity to be tokenised as DICTSEPARATOR and no errors
          // to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 1);
          CHECK(tokens[0].getType() == TokenType::DICTSEPARATOR);
          CHECK(tokens[0].getValue() == ",");
          CHECK(tokens[0].getStartLineNo() == lineNo);
          CHECK(tokens[0].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with pair separator
      WHEN("Singularity is a pair separator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ":", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors and singularity tokenised as pair separator") {
          // Expect singularity to be tokenised as PAIRSEPARATOR and no errors
          // to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 1);
          CHECK(tokens[0].getType() == TokenType::PAIRSEPARATOR);
          CHECK(tokens[0].getValue() == ":");
          CHECK(tokens[0].getStartLineNo() == lineNo);
          CHECK(tokens[0].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with member access operator
      WHEN("Singularity is a member access operator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ".", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
              "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with invalid character
      WHEN("Singularity is an invalid character") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "@", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter or separator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with invalid length singularity
      WHEN("Singularity is an invalid length") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ":.", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Unexpected singularity length - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }
    }
  }

  GIVEN("In single identifier state") {
    // Get into the SINGLEIDENTIFIER state
    lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "identifier", 1, 1));

    // Set up line and char numbers for next lexeme insertion
    const unsigned int lineNo = 1;
    const unsigned int charNo = 11;

    // Transition out of state with IDENTIFIER lexeme
    WHEN("Transition out of state with identifier lexeme") {
      lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "identifier", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("An error and no tokens are produced") {
        // Expect an error and no tokens
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Two consecutive IDENTIFIER lexemes received - aborting evaluation");
        CHECK(errors[0].getLineNo() == lineNo);
        CHECK(errors[0].getCharNo() == charNo);
        CHECK(tokens.empty());
      }
    }

    // Transition out of state with WHITESPACE lexeme
    WHEN("Transition out of state with whitespace lexeme") {
      lexemes.push_back(Lexeme(LexemeType::WHITESPACE, " ", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors and identifier tokenised as value") {
        // Expect identifier to be tokenised as a VALUE token and no errors
        // to be produced
        CHECK(errors.empty());
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].getType() == TokenType::VALUE);
        CHECK(tokens[0].getValue() == "identifier");
        CHECK(tokens[0].getStartLineNo() == 1);
        CHECK(tokens[0].getStartCharNo() == 1);
      }
    }

    // Transition out of state with COMMENT lexeme
    WHEN("Transition out of state with comment lexeme") {
      lexemes.push_back(Lexeme(LexemeType::COMMENT, "/*comment*/", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors and identifier tokenised as value") {
        // Expect identifier to be tokenised as a VALUE token and no errors
        // to be produced
        CHECK(errors.empty());
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].getType() == TokenType::VALUE);
        CHECK(tokens[0].getValue() == "identifier");
        CHECK(tokens[0].getStartLineNo() == 1);
        CHECK(tokens[0].getStartCharNo() == 1);
      }
    }

    // Transition out of state with STRING lexeme
    WHEN("Transition out of state with string lexeme") {
      lexemes.push_back(Lexeme(LexemeType::STRING, "a string", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors, identifier tokenised as value and string tokenised as string") {
        // Expect identifier to be tokenised as a VALUE token, string to be
        // tokenised as a STRING token and no errors to be produced
        CHECK(errors.empty());
        REQUIRE(tokens.size() == 2);
        CHECK(tokens[0].getType() == TokenType::VALUE);
        CHECK(tokens[0].getValue() == "identifier");
        CHECK(tokens[0].getStartLineNo() == 1);
        CHECK(tokens[0].getStartCharNo() == 1);
        CHECK(tokens[1].getType() == TokenType::STRING);
        CHECK(tokens[1].getValue() == "a string");
        CHECK(tokens[1].getStartLineNo() == lineNo);
        CHECK(tokens[1].getStartCharNo() == charNo);
      }
    }

    // Transition out of state with SINGULARITY lexeme
    WHEN("Transition out of state with singularity lexeme") {
      // Transition out of state with opening dict delimiter
      WHEN("Singularity is an opening dict delimiter") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "{", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors, identifier tokenised as value and singularity tokenised as opening dict delimiter") {
          // Expect identifier to be tokenised as a VALUE token, singularity to
          // be tokenised as a DICTDELIMOPEN token and no errors to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 2);
          CHECK(tokens[0].getType() == TokenType::VALUE);
          CHECK(tokens[0].getValue() == "identifier");
          CHECK(tokens[0].getStartLineNo() == 1);
          CHECK(tokens[0].getStartCharNo() == 1);
          CHECK(tokens[1].getType() == TokenType::DICTDELIMOPEN);
          CHECK(tokens[1].getValue() == "{");
          CHECK(tokens[1].getStartLineNo() == lineNo);
          CHECK(tokens[1].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with closing dict delimiter
      WHEN("Singularity is a closing dict delimiter") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "}", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors, identifier tokenised as value and singularity tokenised as closing dict delimiter") {
          // Expect identifier to be tokenised as a VALUE token, singularity to
          // be tokenised as a DICTDELIMCLOSE token and no errors to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 2);
          CHECK(tokens[0].getType() == TokenType::VALUE);
          CHECK(tokens[0].getValue() == "identifier");
          CHECK(tokens[0].getStartLineNo() == 1);
          CHECK(tokens[0].getStartCharNo() == 1);
          CHECK(tokens[1].getType() == TokenType::DICTDELIMCLOSE);
          CHECK(tokens[1].getValue() == "}");
          CHECK(tokens[1].getStartLineNo() == lineNo);
          CHECK(tokens[1].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with dict separator
      WHEN("Singularity is a dict separator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ",", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors, identifier tokenised as value and singularity tokenised as dict separator") {
          // Expect identifier to be tokenised as a VALUE token, singularity to
          // be tokenised as a DICTSEPARATOR token and no errors to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 2);
          CHECK(tokens[0].getType() == TokenType::VALUE);
          CHECK(tokens[0].getValue() == "identifier");
          CHECK(tokens[0].getStartLineNo() == 1);
          CHECK(tokens[0].getStartCharNo() == 1);
          CHECK(tokens[1].getType() == TokenType::DICTSEPARATOR);
          CHECK(tokens[1].getValue() == ",");
          CHECK(tokens[1].getStartLineNo() == lineNo);
          CHECK(tokens[1].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with pair separator
      WHEN("Singularity is a pair separator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ":", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("No errors, identifier tokenised as value and singularity tokenised as pair separator") {
          // Expect identifier to be tokenised as a VALUE token, singularity to
          // be tokenised as a PAIRSEPARATOR token and no errors to be produced
          CHECK(errors.empty());
          REQUIRE(tokens.size() == 2);
          CHECK(tokens[0].getType() == TokenType::VALUE);
          CHECK(tokens[0].getValue() == "identifier");
          CHECK(tokens[0].getStartLineNo() == 1);
          CHECK(tokens[0].getStartCharNo() == 1);
          CHECK(tokens[1].getType() == TokenType::PAIRSEPARATOR);
          CHECK(tokens[1].getValue() == ":");
          CHECK(tokens[1].getStartLineNo() == lineNo);
          CHECK(tokens[1].getStartCharNo() == charNo);
        }
      }

      // Transition out of state with member access operator
      WHEN("Singularity is a member access operator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ".", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Premature end of file - expected identifier after member access operator");
          CHECK(errors[0].getLineNo() == 0);
          CHECK(errors[0].getCharNo() == 0);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with invalid character
      WHEN("Singularity is an invalid character") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "@", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Unexpected character - possible mis-typed delimiter, separator, member access operator or identifier - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with invalid length singularity
      WHEN("Singularity is an invalid length") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ":.", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Unexpected singularity length - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }
    }
  }

  GIVEN("In member accessed indentifier state") {
    // Get into the MEMBERACCESSEDIDENTIFIER state
    lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "identifier", 1, 1));
    lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ".", 1, 11));

    // Set up line and char numbers for next lexeme insertion
    const unsigned int lineNo = 1;
    const unsigned int charNo = 12;

    // Transition out of state with IDENTIFIER lexeme
    WHEN("Transition out of state with identifier lexeme") {
      lexemes.push_back(Lexeme(LexemeType::IDENTIFIER, "identifier", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("No errors and full identifier tokenised as value") {
        // Expect full identifier to be tokenised as a VALUE token and no
        // errors to be produced
        CHECK(errors.empty());
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].getType() == TokenType::VALUE);
        CHECK(tokens[0].getValue() == "identifier.identifier");
        CHECK(tokens[0].getStartLineNo() == 1);
        CHECK(tokens[0].getStartCharNo() == 1);
      }
    }

    // Transition out of state with WHITESPACE lexeme
    WHEN("Transition out of state with whitespace lexeme") {
      lexemes.push_back(Lexeme(LexemeType::WHITESPACE, " ", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("An error and no tokens are produced") {
        // Expect an error and no tokens
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
        CHECK(errors[0].getLineNo() == lineNo);
        CHECK(errors[0].getCharNo() == charNo);
        CHECK(tokens.empty());
      }
    }

    // Transition out of state with COMMENT lexeme
    WHEN("Transition out of state with comment lexeme") {
      lexemes.push_back(Lexeme(LexemeType::COMMENT, "/*comment*/", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("An error and no tokens are produced") {
        // Expect an error and no tokens
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
        CHECK(errors[0].getLineNo() == lineNo);
        CHECK(errors[0].getCharNo() == charNo);
        CHECK(tokens.empty());
      }
    }

    // Transition out of state with STRING lexeme
    WHEN("Transition out of state with string lexeme") {
      lexemes.push_back(Lexeme(LexemeType::STRING, "a string", lineNo, charNo));
      evaluator.evaluate(lexemes, errors, tokens);
      THEN("An error and no tokens are produced") {
        // Expect an error and no tokens
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
          "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
        CHECK(errors[0].getLineNo() == lineNo);
        CHECK(errors[0].getCharNo() == charNo);
        CHECK(tokens.empty());
      }
    }

    // Transition out of state with SINGULARITY lexeme
    WHEN("Transition out of state with singularity lexeme") {
      // Transition out of state with opening dict delimiter
      WHEN("Singularity is an opening dict delimiter") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "{", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with closing dict delimiter
      WHEN("Singularity is a closing dict delimiter") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "}", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with dict separator
      WHEN("Singularity is a dict separator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ",", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with pair separator
      WHEN("Singularity is a pair separator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ":", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with member access operator
      WHEN("Singularity is a member access operator") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ".", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with invalid character
      WHEN("Singularity is an invalid character") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, "@", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }

      // Transition out of state with invalid length singularity
      WHEN("Singularity is an invalid length") {
        lexemes.push_back(Lexeme(LexemeType::SINGULARITY, ":.", lineNo, charNo));
        evaluator.evaluate(lexemes, errors, tokens);
        THEN("An error and no tokens are produced") {
          // Expect an error and no tokens
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
            "EVALUATOR ERROR: Expected identifier after member access operator - aborting evaluation");
          CHECK(errors[0].getLineNo() == lineNo);
          CHECK(errors[0].getCharNo() == charNo);
          CHECK(tokens.empty());
        }
      }
    }
  }
}
