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

/// Try to scan file with empty comments in it and test that it is scanned correctly
SCENARIO("Try to scan file containing empty comments") {
  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;

  scanner.scan("tests/parser/test_files/scanner_tests/emptycomments.def",
    errors,
    lexemes);

  CHECK(errors.empty());
  REQUIRE(lexemes.size() == 43);
  
  for (int lexemeNo = 0; lexemeNo < 43; ++lexemeNo) {
    if (lexemeNo % 2 == 0) {
      switch (lexemeNo) {
        case 0:
        case 32:
        case 42: {
          CHECK(lexemes[lexemeNo].getType() == LexemeType::COMMENT);
          break;
        }
        default: {
          CHECK(lexemes[lexemeNo].getType() == LexemeType::IDENTIFIER);
          break;
        }
      }
    } else {
      CHECK(lexemes[lexemeNo].getType() == LexemeType::WHITESPACE);
    }
  }

  CHECK(lexemes[ 0].getString() == "/**/");
  CHECK(lexemes[ 2].getString() == "Check");
  CHECK(lexemes[ 4].getString() == "that");
  CHECK(lexemes[ 6].getString() == "blank");
  CHECK(lexemes[ 8].getString() == "comments");

  CHECK(lexemes[10].getString() == "work");
  CHECK(lexemes[12].getString() == "at");
  CHECK(lexemes[14].getString() == "the");
  CHECK(lexemes[16].getString() == "start");
  CHECK(lexemes[18].getString() == "of");

  CHECK(lexemes[20].getString() == "a");
  CHECK(lexemes[22].getString() == "file");
  CHECK(lexemes[24].getString() == "and");
  CHECK(lexemes[26].getString() == "in");
  CHECK(lexemes[28].getString() == "the");

  CHECK(lexemes[30].getString() == "middle");
  CHECK(lexemes[32].getString() == "/**/");
  CHECK(lexemes[34].getString() == "and");
  CHECK(lexemes[36].getString() == "at");
  CHECK(lexemes[38].getString() == "the");

  CHECK(lexemes[40].getString() == "end");
  CHECK(lexemes[42].getString() == "/**/");
}

/// Try to scan a file containing a valid definition which covers as many
/// transitions and states as possible
SCENARIO("Try to scan a valid definition which covers as many transitions and states as possible") {
  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;

  scanner.scan("tests/parser/test_files/scanner_tests/allerrorfreetransitionsallstates.def",
    errors,
    lexemes);

  CHECK(errors.empty());
  REQUIRE(lexemes.size() == 31);

  CHECK(lexemes[ 0].getString() == "/*This file attempts to cover as many states and transitions as possible using a syntactically valid definition file ** */");
  CHECK(lexemes[ 0].getType() == LexemeType::COMMENT);
  CHECK(lexemes[ 0].getStartLineNo() == 1);
  CHECK(lexemes[ 0].getStartCharNo() == 1);
  
  CHECK(lexemes[ 1].getString() == "\n 	\n");
  CHECK(lexemes[ 1].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[ 1].getStartLineNo() == 1);
  CHECK(lexemes[ 1].getStartCharNo() == 123);
  
  CHECK(lexemes[ 2].getString() == "{");
  CHECK(lexemes[ 2].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[ 2].getStartLineNo() == 3);
  CHECK(lexemes[ 2].getStartCharNo() == 1);
  
  CHECK(lexemes[ 3].getString() == "description");
  CHECK(lexemes[ 3].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[ 3].getStartLineNo() == 3);
  CHECK(lexemes[ 3].getStartCharNo() == 2);
  
  CHECK(lexemes[ 4].getString() == " ");
  CHECK(lexemes[ 4].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[ 4].getStartLineNo() == 3);
  CHECK(lexemes[ 4].getStartCharNo() == 13);
  
  CHECK(lexemes[ 5].getString() == ":");
  CHECK(lexemes[ 5].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[ 5].getStartLineNo() == 3);
  CHECK(lexemes[ 5].getStartCharNo() == 14);
  
  CHECK(lexemes[ 6].getString() == "All states, all transitions test");
  CHECK(lexemes[ 6].getType() == LexemeType::STRING);
  CHECK(lexemes[ 6].getStartLineNo() == 3);
  CHECK(lexemes[ 6].getStartCharNo() == 15);
  
  CHECK(lexemes[ 7].getString() == ",");
  CHECK(lexemes[ 7].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[ 7].getStartLineNo() == 3);
  CHECK(lexemes[ 7].getStartCharNo() == 49);
  
  CHECK(lexemes[ 8].getString() == "\n  ");
  CHECK(lexemes[ 8].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[ 8].getStartLineNo() == 3);
  CHECK(lexemes[ 8].getStartCharNo() == 50);
  
  CHECK(lexemes[ 9].getString() == "file with spaces.def");
  CHECK(lexemes[ 9].getType() == LexemeType::STRING);
  CHECK(lexemes[ 9].getStartLineNo() == 4);
  CHECK(lexemes[ 9].getStartCharNo() == 3);
  
  CHECK(lexemes[10].getString() == " ");
  CHECK(lexemes[10].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[10].getStartLineNo() == 4);
  CHECK(lexemes[10].getStartCharNo() == 25);
  
  CHECK(lexemes[11].getString() == ":");
  CHECK(lexemes[11].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[11].getStartLineNo() == 4);
  CHECK(lexemes[11].getStartCharNo() == 26);

  CHECK(lexemes[12].getString() == " ");
  CHECK(lexemes[12].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[12].getStartLineNo() == 4);
  CHECK(lexemes[12].getStartCharNo() == 27);

  CHECK(lexemes[13].getString() == "file_with_spaces");
  CHECK(lexemes[13].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[13].getStartLineNo() == 4);
  CHECK(lexemes[13].getStartCharNo() == 28);

  CHECK(lexemes[14].getString() == "\n  ");
  CHECK(lexemes[14].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[14].getStartLineNo() == 4);
  CHECK(lexemes[14].getStartCharNo() == 44);

  CHECK(lexemes[15].getString() == "inputs");
  CHECK(lexemes[15].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[15].getStartLineNo() == 5);
  CHECK(lexemes[15].getStartCharNo() == 3);

  CHECK(lexemes[16].getString() == ":");
  CHECK(lexemes[16].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[16].getStartLineNo() == 5);
  CHECK(lexemes[16].getStartCharNo() == 9);

  CHECK(lexemes[17].getString() == "{");
  CHECK(lexemes[17].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[17].getStartLineNo() == 5);
  CHECK(lexemes[17].getStartCharNo() == 10);

  CHECK(lexemes[18].getString() == "\n    ");
  CHECK(lexemes[18].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[18].getStartLineNo() == 5);
  CHECK(lexemes[18].getStartCharNo() == 11);

  CHECK(lexemes[19].getString() == "/*These are inputs*/");
  CHECK(lexemes[19].getType() == LexemeType::COMMENT);
  CHECK(lexemes[19].getStartLineNo() == 6);
  CHECK(lexemes[19].getStartCharNo() == 5);

  CHECK(lexemes[20].getString() == "\n    ");
  CHECK(lexemes[20].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[20].getStartLineNo() == 6);
  CHECK(lexemes[20].getStartCharNo() == 25);

  CHECK(lexemes[21].getString() == "clock");
  CHECK(lexemes[21].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[21].getStartLineNo() == 7);
  CHECK(lexemes[21].getStartCharNo() == 5);

  CHECK(lexemes[22].getString() == " ");
  CHECK(lexemes[22].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[22].getStartLineNo() == 7);
  CHECK(lexemes[22].getStartCharNo() == 10);

  CHECK(lexemes[23].getString() == ":");
  CHECK(lexemes[23].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[23].getStartLineNo() == 7);
  CHECK(lexemes[23].getStartCharNo() == 11);

  CHECK(lexemes[24].getString() == " ");
  CHECK(lexemes[24].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[24].getStartLineNo() == 7);
  CHECK(lexemes[24].getStartCharNo() == 12);

  CHECK(lexemes[25].getString() == "false");
  CHECK(lexemes[25].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[25].getStartLineNo() == 7);
  CHECK(lexemes[25].getStartCharNo() == 13);

  CHECK(lexemes[26].getString() == "/*This input is a clock*/");
  CHECK(lexemes[26].getType() == LexemeType::COMMENT);
  CHECK(lexemes[26].getStartLineNo() == 7);
  CHECK(lexemes[26].getStartCharNo() == 18);

  CHECK(lexemes[27].getString() == "\n  ");
  CHECK(lexemes[27].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[27].getStartLineNo() == 7);
  CHECK(lexemes[27].getStartCharNo() == 43);

  CHECK(lexemes[28].getString() == "}");
  CHECK(lexemes[28].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[28].getStartLineNo() == 8);
  CHECK(lexemes[28].getStartCharNo() == 3);

  CHECK(lexemes[29].getString() == "\n");
  CHECK(lexemes[29].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[29].getStartLineNo() == 8);
  CHECK(lexemes[29].getStartCharNo() == 4);

  CHECK(lexemes[30].getString() == "}");
  CHECK(lexemes[30].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[30].getStartLineNo() == 9);
  CHECK(lexemes[30].getStartCharNo() == 1);
}

/// Try to scan a file containing an invalid definition file which covers all
/// transitions and states but, although the scanner should not produce any
/// errors, is not syntactically valid
SCENARIO("Try to scan an invalid definition which covers all transitions and states") {
  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;

  scanner.scan("tests/parser/test_files/scanner_tests/alltransitionsallstates.def",
    errors,
    lexemes);

  CHECK(errors.empty());
  REQUIRE(lexemes.size() == 42);

  CHECK(lexemes[0].getString() == "/*This file covers all states and transitions but is not a syntactically valid definition file, though the scanner should not give any errors ** */");
  CHECK(lexemes[0].getType() == LexemeType::COMMENT);
  CHECK(lexemes[0].getStartLineNo() == 1);
  CHECK(lexemes[0].getStartCharNo() == 1);

  CHECK(lexemes[1].getString() == "\n 	\n");
  CHECK(lexemes[1].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[1].getStartLineNo() == 1);
  CHECK(lexemes[1].getStartCharNo() == 148);

  CHECK(lexemes[2].getString() == "{");
  CHECK(lexemes[2].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[2].getStartLineNo() == 3);
  CHECK(lexemes[2].getStartCharNo() == 1);

  CHECK(lexemes[3].getString() == "description");
  CHECK(lexemes[3].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[3].getStartLineNo() == 3);
  CHECK(lexemes[3].getStartCharNo() == 2);

  CHECK(lexemes[4].getString() == " ");
  CHECK(lexemes[4].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[4].getStartLineNo() == 3);
  CHECK(lexemes[4].getStartCharNo() == 13);

  CHECK(lexemes[5].getString() == ":");
  CHECK(lexemes[5].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[5].getStartLineNo() == 3);
  CHECK(lexemes[5].getStartCharNo() == 14);

  CHECK(lexemes[6].getString() == "All states, all transitions test");
  CHECK(lexemes[6].getType() == LexemeType::STRING);
  CHECK(lexemes[6].getStartLineNo() == 3);
  CHECK(lexemes[6].getStartCharNo() == 15);

  CHECK(lexemes[7].getString() == ",");
  CHECK(lexemes[7].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[7].getStartLineNo() == 3);
  CHECK(lexemes[7].getStartCharNo() == 49);

  CHECK(lexemes[8].getString() == "\n  ");
  CHECK(lexemes[8].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[8].getStartLineNo() == 3);
  CHECK(lexemes[8].getStartCharNo() == 50);

  CHECK(lexemes[9].getString() == "file with spaces.def");
  CHECK(lexemes[9].getType() == LexemeType::STRING);
  CHECK(lexemes[9].getStartLineNo() == 4);
  CHECK(lexemes[9].getStartCharNo() == 3);

  CHECK(lexemes[10].getString() == " ");
  CHECK(lexemes[10].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[10].getStartLineNo() == 4);
  CHECK(lexemes[10].getStartCharNo() == 25);

  CHECK(lexemes[11].getString() == ":");
  CHECK(lexemes[11].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[11].getStartLineNo() == 4);
  CHECK(lexemes[11].getStartCharNo() == 26);

  CHECK(lexemes[12].getString() == " ");
  CHECK(lexemes[12].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[12].getStartLineNo() == 4);
  CHECK(lexemes[12].getStartCharNo() == 27);

  CHECK(lexemes[13].getString() == "file_with_spaces");
  CHECK(lexemes[13].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[13].getStartLineNo() == 4);
  CHECK(lexemes[13].getStartCharNo() == 28);

  CHECK(lexemes[14].getString() == "\n  ");
  CHECK(lexemes[14].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[14].getStartLineNo() == 4);
  CHECK(lexemes[14].getStartCharNo() == 44);

  CHECK(lexemes[15].getString() == "inputs");
  CHECK(lexemes[15].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[15].getStartLineNo() == 5);
  CHECK(lexemes[15].getStartCharNo() == 3);

  CHECK(lexemes[16].getString() == ":");
  CHECK(lexemes[16].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[16].getStartLineNo() == 5);
  CHECK(lexemes[16].getStartCharNo() == 9);

  CHECK(lexemes[17].getString() == "{");
  CHECK(lexemes[17].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[17].getStartLineNo() == 5);
  CHECK(lexemes[17].getStartCharNo() == 10);

  CHECK(lexemes[18].getString() == "\n    ");
  CHECK(lexemes[18].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[18].getStartLineNo() == 5);
  CHECK(lexemes[18].getStartCharNo() == 11);

  CHECK(lexemes[19].getString() == "/*These are inputs*/");
  CHECK(lexemes[19].getType() == LexemeType::COMMENT);
  CHECK(lexemes[19].getStartLineNo() == 6);
  CHECK(lexemes[19].getStartCharNo() == 5);

  CHECK(lexemes[20].getString() == "\n    ");
  CHECK(lexemes[20].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[20].getStartLineNo() == 6);
  CHECK(lexemes[20].getStartCharNo() == 25);

  CHECK(lexemes[21].getString() == "clock");
  CHECK(lexemes[21].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[21].getStartLineNo() == 7);
  CHECK(lexemes[21].getStartCharNo() == 5);

  CHECK(lexemes[22].getString() == " ");
  CHECK(lexemes[22].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[22].getStartLineNo() == 7);
  CHECK(lexemes[22].getStartCharNo() == 10);

  CHECK(lexemes[23].getString() == ":");
  CHECK(lexemes[23].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[23].getStartLineNo() == 7);
  CHECK(lexemes[23].getStartCharNo() == 11);

  CHECK(lexemes[24].getString() == " ");
  CHECK(lexemes[24].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[24].getStartLineNo() == 7);
  CHECK(lexemes[24].getStartCharNo() == 12);

  CHECK(lexemes[25].getString() == "false");
  CHECK(lexemes[25].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[25].getStartLineNo() == 7);
  CHECK(lexemes[25].getStartCharNo() == 13);

  CHECK(lexemes[26].getString() == "/*This input is a clock*/");
  CHECK(lexemes[26].getType() == LexemeType::COMMENT);
  CHECK(lexemes[26].getStartLineNo() == 7);
  CHECK(lexemes[26].getStartCharNo() == 18);

  CHECK(lexemes[27].getString() == "\n  ");
  CHECK(lexemes[27].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[27].getStartLineNo() == 7);
  CHECK(lexemes[27].getStartCharNo() == 43);

  CHECK(lexemes[28].getString() == "}");
  CHECK(lexemes[28].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[28].getStartLineNo() == 8);
  CHECK(lexemes[28].getStartCharNo() == 3);

  CHECK(lexemes[29].getString() == "\n");
  CHECK(lexemes[29].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[29].getStartLineNo() == 8);
  CHECK(lexemes[29].getStartCharNo() == 4);

  CHECK(lexemes[30].getString() == "}");
  CHECK(lexemes[30].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[30].getStartLineNo() == 9);
  CHECK(lexemes[30].getStartCharNo() == 1);

  CHECK(lexemes[31].getString() == "\n\n");
  CHECK(lexemes[31].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[31].getStartLineNo() == 9);
  CHECK(lexemes[31].getStartCharNo() == 2);

  CHECK(lexemes[32].getString() == "/");
  CHECK(lexemes[32].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[32].getStartLineNo() == 11);
  CHECK(lexemes[32].getStartCharNo() == 1);

  CHECK(lexemes[33].getString() == " ");
  CHECK(lexemes[33].getType() == LexemeType::WHITESPACE);
  CHECK(lexemes[33].getStartLineNo() == 11);
  CHECK(lexemes[33].getStartCharNo() == 2);

  CHECK(lexemes[34].getString() == "/");
  CHECK(lexemes[34].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[34].getStartLineNo() == 11);
  CHECK(lexemes[34].getStartCharNo() == 3);

  CHECK(lexemes[35].getString() == "/");
  CHECK(lexemes[35].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[35].getStartLineNo() == 11);
  CHECK(lexemes[35].getStartCharNo() == 4);

  CHECK(lexemes[36].getString() == "");
  CHECK(lexemes[36].getType() == LexemeType::STRING);
  CHECK(lexemes[36].getStartLineNo() == 11);
  CHECK(lexemes[36].getStartCharNo() == 5);

  CHECK(lexemes[37].getString() == "/");
  CHECK(lexemes[37].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[37].getStartLineNo() == 11);
  CHECK(lexemes[37].getStartCharNo() == 7);

  CHECK(lexemes[38].getString() == ";");
  CHECK(lexemes[38].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[38].getStartLineNo() == 11);
  CHECK(lexemes[38].getStartCharNo() == 8);

  CHECK(lexemes[39].getString() == "/");
  CHECK(lexemes[39].getType() == LexemeType::SINGULARITY);
  CHECK(lexemes[39].getStartLineNo() == 11);
  CHECK(lexemes[39].getStartCharNo() == 9);

  CHECK(lexemes[40].getString() == "identifier");
  CHECK(lexemes[40].getType() == LexemeType::IDENTIFIER);
  CHECK(lexemes[40].getStartLineNo() == 11);
  CHECK(lexemes[40].getStartCharNo() == 10);

  CHECK(lexemes[41].getString() == "");
  CHECK(lexemes[41].getType() == LexemeType::STRING);
  CHECK(lexemes[41].getStartLineNo() == 11);
  CHECK(lexemes[41].getStartCharNo() == 20);
}

/// Test that the scanner works correctly when the file ends in any state
SCENARIO("Test that the scanner works correctly when the file ends in any state") {
  Scanner scanner;

  std::vector<Lexeme> lexemes;
  std::vector<ParserError> errors;

  WHEN("State at end of file is IDLE") {
    scanner.scan( "tests/parser/test_files/scanner_tests/endoffileallstates/eofidle.def",
                  errors,
                  lexemes);

    // It should be fine to end the file in the IDLE state, so expect no errors
    // and 3 lexemes
    CHECK(errors.empty());
    REQUIRE(lexemes.size() == 3);

    CHECK(lexemes[0].getString() == "idle");
    CHECK(lexemes[0].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[0].getStartLineNo() == 1);
    CHECK(lexemes[0].getStartCharNo() == 1);

    CHECK(lexemes[1].getString() == " ");
    CHECK(lexemes[1].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[1].getStartLineNo() == 1);
    CHECK(lexemes[1].getStartCharNo() == 5);

    CHECK(lexemes[2].getString() == ";");
    CHECK(lexemes[2].getType() == LexemeType::SINGULARITY);
    CHECK(lexemes[2].getStartLineNo() == 1);
    CHECK(lexemes[2].getStartCharNo() == 6);
  }

  WHEN("State at end of file is IDENTIFIERPROCESSING") {
    CHECK(errors.empty());
    REQUIRE(lexemes.empty());
    scanner.scan( "tests/parser/test_files/scanner_tests/endoffileallstates/eofidentifierprocessing.def",
                  errors,
                  lexemes);

    // It should be fine to end the file in the IDENTIFIERPROCESSING state, so
    // expect no errors and 3 lexemes
    CHECK(errors.empty());
    REQUIRE(lexemes.size() == 3);

    CHECK(lexemes[0].getString() == "identifier");
    CHECK(lexemes[0].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[0].getStartLineNo() == 1);
    CHECK(lexemes[0].getStartCharNo() == 1);

    CHECK(lexemes[1].getString() == " ");
    CHECK(lexemes[1].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[1].getStartLineNo() == 1);
    CHECK(lexemes[1].getStartCharNo() == 11);

    CHECK(lexemes[2].getString() == "processing");
    CHECK(lexemes[2].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[2].getStartLineNo() == 1);
    CHECK(lexemes[2].getStartCharNo() == 12);
  }

  WHEN("State at end of file is STRINGPROCESSING") {
    scanner.scan( "tests/parser/test_files/scanner_tests/endoffileallstates/eofstringprocessing.def",
                  errors,
                  lexemes);

    // It should not be fine to end the file in the STRINGPROCESSING state, so
    // expect 1 error and no lexemes
    REQUIRE(errors.size() == 1);
    CHECK(lexemes.empty());

    CHECK(errors[0].getErrorDescription() ==
        "SCANNER ERROR: String unterminated by end of file");
    CHECK(errors[0].getLineNo() == 1);
    CHECK(errors[0].getCharNo() == 1);
  }

  WHEN("State at end of file is COMMENTPROCESSING") {
    scanner.scan( "tests/parser/test_files/scanner_tests/endoffileallstates/eofcommentprocessing.def",
                  errors,
                  lexemes);

    // It should not be fine to end the file in the COMMENTPROCESSING state, so
    // expect 1 error and no lexemes
    REQUIRE(errors.size() == 1);
    CHECK(lexemes.empty());

    CHECK(errors[0].getErrorDescription() ==
        "SCANNER ERROR: Comment unterminated by end of file");
    CHECK(errors[0].getLineNo() == 1);
    CHECK(errors[0].getCharNo() == 1);
  }

  WHEN("State at end of file is WHITESPACEPROCESSING") {
    scanner.scan( "tests/parser/test_files/scanner_tests/endoffileallstates/eofwhitespaceprocessing.def",
                  errors,
                  lexemes);

    // It should be fine to end the file in the WHITESPACEPROCESSING state, so
    // expect no errors and 4 lexemes
    CHECK(errors.empty());
    REQUIRE(lexemes.size() == 4);

    CHECK(lexemes[0].getString() == "whitespace");
    CHECK(lexemes[0].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[0].getStartLineNo() == 1);
    CHECK(lexemes[0].getStartCharNo() == 1);

    CHECK(lexemes[1].getString() == " ");
    CHECK(lexemes[1].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[1].getStartLineNo() == 1);
    CHECK(lexemes[1].getStartCharNo() == 11);

    CHECK(lexemes[2].getString() == "processing");
    CHECK(lexemes[2].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[2].getStartLineNo() == 1);
    CHECK(lexemes[2].getStartCharNo() == 12);

    CHECK(lexemes[3].getString() == "    ");
    CHECK(lexemes[3].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[3].getStartLineNo() == 1);
    CHECK(lexemes[3].getStartCharNo() == 22);
  }

  WHEN("State at end of file is POTENTIALBEGINCOMMENT") {
    scanner.scan( "tests/parser/test_files/scanner_tests/endoffileallstates/eofpotentialbegincomment.def",
                  errors,
                  lexemes);

    // It should be fine to end the file in the POTENTIALBEGINCOMMENT state, so
    // expect no errors and 7 lexemes
    CHECK(errors.empty());
    REQUIRE(lexemes.size() == 7);

    CHECK(lexemes[0].getString() == "potential");
    CHECK(lexemes[0].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[0].getStartLineNo() == 1);
    CHECK(lexemes[0].getStartCharNo() == 1);

    CHECK(lexemes[1].getString() == " ");
    CHECK(lexemes[1].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[1].getStartLineNo() == 1);
    CHECK(lexemes[1].getStartCharNo() == 10);

    CHECK(lexemes[2].getString() == "begin");
    CHECK(lexemes[2].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[2].getStartLineNo() == 1);
    CHECK(lexemes[2].getStartCharNo() == 11);

    CHECK(lexemes[3].getString() == " ");
    CHECK(lexemes[3].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[3].getStartLineNo() == 1);
    CHECK(lexemes[3].getStartCharNo() == 16);

    CHECK(lexemes[4].getString() == "comment");
    CHECK(lexemes[4].getType() == LexemeType::IDENTIFIER);
    CHECK(lexemes[4].getStartLineNo() == 1);
    CHECK(lexemes[4].getStartCharNo() == 17);

    CHECK(lexemes[5].getString() == " ");
    CHECK(lexemes[5].getType() == LexemeType::WHITESPACE);
    CHECK(lexemes[5].getStartLineNo() == 1);
    CHECK(lexemes[5].getStartCharNo() == 24);

    CHECK(lexemes[6].getString() == "/");
    CHECK(lexemes[6].getType() == LexemeType::SINGULARITY);
    CHECK(lexemes[6].getStartLineNo() == 1);
    CHECK(lexemes[6].getStartCharNo() == 25);
  }

  WHEN("State at end of file is POTENTIALENDCOMMENT") {
    scanner.scan( "tests/parser/test_files/scanner_tests/endoffileallstates/eofpotentialendcomment.def",
                  errors,
                  lexemes);

    // It should not be fine to end the file in the POTENTIALENDCOMMENT state,
    // so expect 1 error and no lexemes
    REQUIRE(errors.size() == 1);
    CHECK(lexemes.empty());

    CHECK(errors[0].getErrorDescription() ==
        "SCANNER ERROR: Comment unterminated by end of file");
    CHECK(errors[0].getLineNo() == 1);
    CHECK(errors[0].getCharNo() == 1);
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
