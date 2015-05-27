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
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (char(character) == '"') {
        // The string delimiter starts a string lexeme, which will be
        // unterminated at the end of the file giving 1 error and 0 lexemes
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: String unterminated by end of file");
        CHECK(lexemes.empty());
      } else {
        // All other valid characters should give 0 errors and 1 lexeme
        CHECK(errors.empty());
        CHECK(lexemes.size() == 1);
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.empty());
    }
  }
}

/// Create start character files for all 256 characters and test that they get
/// scanned correctly
SCENARIO("Create files starting with each character and run the scanner on these") {
  const std::string folderName = "charactersatstart";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("", " at start", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (char(character) == '"') {
        // The string delimiter starts a string lexeme, which will be
        // unterminated at the end of the file giving 1 error and 0 lexemes
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: String unterminated by end of file");
        CHECK(lexemes.empty());
      } else {
        CHECK(errors.empty());
        if (isWhitespaceChar(character)) {
          // Whitespace characters should combine with the space at the start of
          // " at start" and hence give 0 errors and 4 lexemes
          CHECK(lexemes.size() == 4);
        } else {
          // All other valid characters should not combine with the space at the
          // start of " at start" and hence give 0 errors and 5 lexemes
          CHECK(lexemes.size() == 5);
        }
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.empty());
    }
  }
}

/// Create files for all 256 characters in the IDLE state and test that they
/// get scanned correctly. Also serves as a test of all the characters being
/// the last in the file
SCENARIO("Create files with each character input in the IDLE state and run the scanner on these") {
  const std::string folderName = "charactersallstates/idle";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("character:", "", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (char(character) == '"') {
        // The string delimiter starts a string lexeme, which will be
        // unterminated at the end of the file giving 1 error and 2 lexemes
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: String unterminated by end of file");
        CHECK(lexemes.size() == 2);
      } else {
        // All other characters should give 0 errors and 3 lexemes
        CHECK(errors.empty());
        CHECK(lexemes.size() == 3);
      }
    } else {
      // All invalid characters should give 1 error and 2 lexemes before this
      // error is produced and scanning aborted
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.size() == 2);
    }
  }
}

/// Create files for all 256 characters in the IDENTIFIERPROCESSING state and
/// test that they get scanned correctly
SCENARIO("Create files with each character input in the IDENTIFIERPROCESSING state and run the scanner on these") {
  const std::string folderName = "charactersallstates/identifierprocessing";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("identStart", "End", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (char(character) == '"') {
        // The string delimiter starts a string lexeme, which will be
        // unterminated at the end of the file giving 1 error and 1 lexemes
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: String unterminated by end of file");
        CHECK(lexemes.size() == 1);
      } else {
        CHECK(errors.empty());
        if (isCharacterChar(character)) {
          // Valid identifier characters should combine with the pre- and
          // post- strings and hence give 0 errors and 1 lexeme
          CHECK(lexemes.size() == 1);
        } else {
          // All other valid characters should not combine with the pre- and
          // post- strings and hence give 0 errors and 3 lexemes
          CHECK(lexemes.size() == 3);
        }
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.empty());
    }
  }
}

/// Create files for all 256 characters in the STRINGPROCESSING state and
/// test that they get scanned correctly
SCENARIO("Create files with each character input in the STRINGPROCESSING state and run the scanner on these") {
  const std::string folderName = "charactersallstates/stringprocessing";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("\"string:", "\"", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (isStringChar(character)) {
        if (char(character) == '"') {
          // The string delimiter closes the string lexeme, and then the
          // post-string string delimiter starts another string lexeme, which
          // will be unterminated at the end of the file giving 1 error and 1
          // lexeme
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
              "SCANNER ERROR: String unterminated by end of file");
        } else {
          // All other valid string characters will become part of the string
          // lexeme, and hence should give 0 errors and 1 lexeme
          CHECK(errors.empty());
        }
        CHECK(lexemes.size() == 1);
      } else {
        // Invalid string characters should give 1 error and 0 lexemes
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: Invalid character in string - aborted scanning");
        CHECK(lexemes.empty());
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.empty());
    }
  }
}

/// Create files for all 256 characters in the COMMENTPROCESSING state and
/// test that they get scanned correctly
SCENARIO("Create files with each character input in the COMMENTPROCESSING state and run the scanner on these") {
  const std::string folderName = "charactersallstates/commentprocessing";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("/*comment:", "", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    REQUIRE(errors.size() == 1);
    if (isValidCharacter(character)) {
      if (isCommentChar(character)) {
        // All valid comment characters will become part of the unterminated
        // comment and hence should give 1 error and 0 lexemes
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: Comment unterminated by end of file");
      } else {
        // All invalid comment characters should give 1 error and 0 lexemes
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: Invalid character in comment - aborted scanning");
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
    }
    CHECK(lexemes.empty());
  }
}

/// Create files for all 256 characters in the WHITESPACEPROCESSING state and
/// test that they get scanned correctly
SCENARIO("Create files with each character input in the WHITESPACEPROCESSING state and run the scanner on these") {
  const std::string folderName = "charactersallstates/whitespaceprocessing";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("   ", "  ", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (char(character) == '"') {
        // The string delimiter starts a string lexeme, which will be
        // unterminated at the end of the file giving 1 error and 1 lexemes
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: String unterminated by end of file");
        CHECK(lexemes.size() == 1);
      } else {
        CHECK(errors.empty());
        if (isWhitespaceChar(character)) {
          // Valid whitespace characters should combine with the pre- and
          // post- strings and hence give 0 errors and 1 lexeme
          CHECK(lexemes.size() == 1);
        } else {
          // All other valid characters should not combine with the pre- and
          // post- strings and hence give 0 errors and 3 lexemes
          CHECK(lexemes.size() == 3);
        }
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.empty());
    }
  }
}

/// Create files for all 256 characters in the POTENTIALBEGINCOMMENT state and
/// test that they get scanned correctly
SCENARIO("Create files with each character input in the POTENTIALBEGINCOMMENT state and run the scanner on these") {
  const std::string folderName = "charactersallstates/potentialbegincomment";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("/", "", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (char(character) == '*') {
        // The * character starts a comment lexeme, which will be unterminated
        // at the end of the file giving 1 error and 0 lexemes
        REQUIRE(errors.size() == 1);
        CHECK(errors[0].getErrorDescription() ==
            "SCANNER ERROR: Comment unterminated by end of file");
        CHECK(lexemes.empty());
      } else {
        if (char(character) == '"') {
          // The string delimiter starts a string lexeme, which will be
          // unterminated at the end of the file giving 1 error and 1 lexemes
          REQUIRE(errors.size() == 1);
          CHECK(errors[0].getErrorDescription() ==
              "SCANNER ERROR: String unterminated by end of file");
          CHECK(lexemes.size() == 1);
        } else {
          // All other valid characters should give 0 errors and 2 lexemes
          CHECK(errors.empty());
          CHECK(lexemes.size() == 2);
        }
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.empty());
    }
  }
}

/// Create files for all 256 characters in the POTENTIALENDCOMMENT state and
/// test that they get scanned correctly
SCENARIO("Create files with each character input in the POTENTIALENDCOMMENT state and run the scanner on these") {
  const std::string folderName = "charactersallstates/potentialendcomment";
  // Make sure the files are all successfully created
  REQUIRE(createOneFilePerCharacter("/*comment*", "", folderName) == 256);

  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;
  // Check that the files return what would be expected
  for (int character = 0; character < 256; ++character) {
    lexemes.clear();
    errors.clear();
    scanner.scan(getCharacterFilename(character, folderName), errors, lexemes);
    if (isValidCharacter(character)) {
      if (char(character) == '/') {
        // The / character closes a comment lexeme, giving 0 errors and 1
        // lexeme
        CHECK(errors.empty());
        CHECK(lexemes.size() == 1);
      } else {
        REQUIRE(errors.size() == 1);
        if (isCommentChar(character)) {
          // All valid comment characters will become part of the unterminated
          // comment and hence should give 1 error and 0 lexemes
          CHECK(errors[0].getErrorDescription() ==
              "SCANNER ERROR: Comment unterminated by end of file");
        } else {
          // All other characters are invalid comment characters and hence give
          // 1 error and 0 lexemes
          CHECK(errors[0].getErrorDescription() ==
              "SCANNER ERROR: Invalid character in comment - aborted scanning");
        }
        CHECK(lexemes.empty());
      }
    } else {
      // All invalid characters should give 1 error and 0 lexemes
      REQUIRE(errors.size() == 1);
      CHECK(errors[0].getErrorDescription() ==
          "SCANNER ERROR: Invalid character in file - aborted scanning");
      CHECK(lexemes.empty());
    }
  }
}

/// Try to scan a non-existent file and check that expected error is returned
SCENARIO("Try to scan non-existent file") {
  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;

  scanner.scan("tests/parser/test_files/scanner_tests/non_existent_file.def",
    errors,
    lexemes);

  // File shouldn't exist, so scanner should give 1 error and 0 lexemes
  REQUIRE(errors.size() == 1);
  CHECK(errors[0].getErrorDescription() == "SCANNER ERROR: File open operation failed - aborted scanning");
  CHECK(lexemes.empty());
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
