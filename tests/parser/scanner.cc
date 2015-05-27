#include <catch.h>

#include "../../src/parser/scanner.h"
#include "../../src/parser/charcheck.h"

#include <fstream>
#include <string>

/// Create the filename (including path) for the single-character definition
/// file containing the specified character
std::string getCharacterFilename(int character) {
  char filenamecstr[256];
  sprintf(  filenamecstr,
            "tests/parser/test_files/scanner_tests/characters/char%03u.def",
            character);
  return std::string(filenamecstr);
}

/// Create single-character files for all 256 characters and test that they get
/// scanned correctly
SCENARIO("Create files containing each character and run the scanner on these") {
  int successes = 0;
  // Create files for all 256 characters, each one containing just the single
  // character
  for (int character = 0; character < 256; ++character) {
    std::ofstream file;
    file.open(getCharacterFilename(character).c_str());
    if (file.is_open()) {
      ++successes;
      file << char(character);
      file.close();
    }
  }
  // Make sure the files exist
  REQUIRE(successes == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return an error and no lexemes if they should, and
  // that the files return a lexeme and no errors if they should
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character), errors, lexemes);
    bool shouldBeErrorless = true;
    if ((!isValidCharacter(character)) || (char(character) == '"')) {
      shouldBeErrorless = false;
    }
    if (shouldBeErrorless) {
      CHECK(errors.empty());
      CHECK(!lexemes.empty());
    }
    else {
      CHECK(!errors.empty());
      CHECK(lexemes.empty());
    }
  }
}

SCENARIO("Scan test.def") {
  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;

  scanner.scan( "tests/parser/test_files/scanner_tests/test.def",
                errors,
                lexemes);
  
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
