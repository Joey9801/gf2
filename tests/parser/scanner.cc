#include <catch.h>

#include "../../src/parser/scanner.h"

SCENARIO("Scan test.def") {
  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;

  scanner.scan("tests/parser/test_files/scanner_tests/test.def", errors, lexemes);
  
  WHEN("Check errors are correct") {
    REQUIRE(errors.size() == 1);
    CHECK(errors[0].getErrorDescription() ==
        "SCANNER ERROR: Invalid character in file - aborted scanning");
  }

  WHEN("Check lexemes are correct") {
    REQUIRE(lexemes.size() == 8);

    CHECK(lexemes[0].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[0].getString() == "hello");
    CHECK(lexemes[0].getStartLineNo() == 1);
    CHECK(lexemes[0].getStartCharNo() == 1);

    CHECK(lexemes[1].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[1].getString() == "	 	 		 	 	 	 ");
    CHECK(lexemes[1].getStartLineNo() == 1);
    CHECK(lexemes[1].getStartCharNo() == 6);

    CHECK(lexemes[2].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[2].getString() == "9909230845");
    CHECK(lexemes[2].getStartLineNo() == 1);
    CHECK(lexemes[2].getStartCharNo() == 19);

    CHECK(lexemes[3].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[3].getString() == " ");
    CHECK(lexemes[3].getStartLineNo() == 1);
    CHECK(lexemes[3].getStartCharNo() == 29);

    CHECK(lexemes[4].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[4].getString() == "sdjflk");
    CHECK(lexemes[4].getStartLineNo() == 1);
    CHECK(lexemes[4].getStartCharNo() == 30);

    CHECK(lexemes[5].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[5].getString() == " ");
    CHECK(lexemes[5].getStartLineNo() == 1);
    CHECK(lexemes[5].getStartCharNo() == 36);

    CHECK(lexemes[6].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[6].getString() == "asjf");
    CHECK(lexemes[6].getStartLineNo() == 1);
    CHECK(lexemes[6].getStartCharNo() == 37);

    CHECK(lexemes[7].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[7].getString() == "      ");
    CHECK(lexemes[7].getStartLineNo() == 1);
    CHECK(lexemes[7].getStartCharNo() == 41);
  }
}
