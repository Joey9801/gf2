#include <catch.h>

#include "../../src/parser/scanner.h"
#include "../../src/parser/charcheck.h"

#include <fstream>
#include <string>

/// Create the filename (including path) for the definition file containing the
/// specified character, within a subfolder of the scanner_test directory which
/// has the specified folder name
std::string getCharacterFilename(int character, std::string testFolderName) {
  char filenamecstr[30];
  sprintf(  filenamecstr,
            "/char%03u.def",
            character);
  std::string filename("tests/parser/test_files/scanner_tests/");
  filename.append(testFolderName);
  filename.append(filenamecstr);
  return filename;
}

/// Create the files for all characters with a specified pre-string and
/// post-string within the specified directory. Return the number of successes,
/// so that the calling function can check all 256 files now exist
int createOneFilePerCharacter(std::string preString, std::string postString,
                              std::string testFolderName) {
  int successes = 0;
  // Create files for all 256 characters, in the specified folder, which
  // contain [preString][character][postString]
  for (int character = 0; character < 256; ++character) {
    std::ofstream file;
    file.open(getCharacterFilename(character, testFolderName).c_str());
    if (file.is_open()) {
      ++successes;
      file << preString << char(character) << postString;
      file.close();
    }
  }
  return successes;
}

/// Create single-character files for all 256 characters and test that they get
/// scanned correctly
SCENARIO("Create files containing each character and run the scanner on these") {
  const std::string folderName = "characters";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("", "", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return an error and no lexemes if they should, and
  // that the files return a lexeme and no errors if they should
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
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
