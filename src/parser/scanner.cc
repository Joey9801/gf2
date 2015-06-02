#include "scanner.h"
#include "charcheck.h"

#include <fstream>

/// Constructor for the Scanner class
Scanner::Scanner() : currentState(ScannerState::START) {
  resetToStartState();
}

/// Scans file and returns a vector of the lexemes it contains
bool Scanner::scan( const std::string filename,
                    std::vector<ParserError>& errors,
                    std::vector<Lexeme>& lexemes) {
  resetToStartState();
  // Clear the vector of lexemes to prevent mixing old and new lexemes
  lexemes.clear();

  // Try to open the file, and make sure it is open before using it
  std::ifstream inf;
  char character;
  inf.open(filename);
  if (inf) {
    bool aborted = false;
    // Repeatedly process characters until the end of the file
    while (!(inf.get(character) == 0) && !aborted) {
      currentLineChar++;
      aborted = !processCharacter(character, errors, lexemes);
      if (character == '\n') {
        currentFileLine++;
        currentLineChar = 0;
      }
      if (currentLineChar == std::numeric_limits<unsigned int>::max()) {
        // If we are unable to count any higher on this line then abort and
        // return an error
        aborted = true;
        errors.push_back(ParserError( addScannerErrorPrefix(
                                          "Could not scan whole line as it was too long - aborted scanning"),
                                      currentFileLine,
                                      currentLineChar));
      }
      if (currentFileLine == std::numeric_limits<unsigned int>::max()) {
        // If we are unable to count any more lines then abort and return
        // an error
        aborted = true;
        errors.push_back(ParserError( addScannerErrorPrefix(
                                          "Could not scan whole file as it had too many lines - aborted scanning"),
                                      currentFileLine,
                                      currentLineChar));
      }
    }
    if (!aborted) {
      // Process the end of the file
      processEndOfFile(errors, lexemes);
    }
    // Close the file
    inf.close();
  } else {
    // Add an error - failed to open file
    errors.push_back(ParserError( addScannerErrorPrefix(
                                      "File open operation failed - aborted scanning")));
  }

  // If there are errors then clear the vector of lexemes to prevent use of
  // a half-complete vector
  if (!errors.empty()) {
    lexemes.clear();
  }

  return errors.empty();
}

/// Processes the next character as an input to the scanner state machine
bool Scanner::processCharacter( const char character,
                                std::vector<ParserError>& errors,
                                std::vector<Lexeme>& lexemes) {
  if (!isValidCharacter(character)) {
    // Add an error - the passed character is not a valid character
    // to be present in a definition file
    errors.push_back(ParserError( addScannerErrorPrefix(
                                      "Invalid character in file - aborted scanning"),
                                  currentFileLine,
                                  currentLineChar));
    // Return false - there is no point in continuing to scan here, the
    // state machine does not define transitions for invalid characters
    return false;
  } else {
    switch (currentState) {
      case ScannerState::IDLE: {
        return idleProcessCharacter(character, errors, lexemes);
      }
      case ScannerState::IDENTIFIERPROCESSING: {
        return identifierProcessCharacter(character, errors, lexemes);
      }
      case ScannerState::STRINGPROCESSING: {
        return stringProcessCharacter(character, errors, lexemes);
      }
      case ScannerState::WHITESPACEPROCESSING: {
        return whitespaceProcessCharacter(character, errors, lexemes);
      }
      case ScannerState::COMMENTPROCESSING: {
        return commentProcessCharacter(character, errors, lexemes);
      }
      case ScannerState::POTENTIALBEGINCOMMENT: {
        return potBeginCommentProcessCharacter(character, errors, lexemes);
      }
      case ScannerState::POTENTIALENDCOMMENT: {
        return potEndCommentProcessCharacter(character, errors, lexemes);
      }
    }
    return false;
  }
}

/// Processes the next character when the current state is IDLE
bool Scanner::idleProcessCharacter( const char character,
                                    std::vector<ParserError>& errors,
                                    std::vector<Lexeme>& lexemes) {
  // Check the character is valid, just to ensure that the state machine is
  // safe - don't report the error as this should be done by the main character
  // processing function
  if (isValidCharacter(character)) {
    // Whatever the character is, it will be the start of a new lexeme
    startNewLexeme();
    // If the character is a ", go to the STRINGPROCESSING state but don't add
    // the character to the current string, as it should not be included
    if (character == '"') {
      currentState = ScannerState::STRINGPROCESSING;
    } else {
      // As the character is not a ", it should be added to the current string
      currentString.push_back(character);
      // If the character is a valid identifier character, go to the
      // IDENTIFIERPROCESSING state
      if (isCharacterChar(character)) {
        currentState = ScannerState::IDENTIFIERPROCESSING;
      } else {
        // If the character is a valid whitespace character, go to the
        // WHITESPACEPROCESSING state
        if (isWhitespaceChar(character)) {
          currentState = ScannerState::WHITESPACEPROCESSING;
        } else {
          // If the character is a /, go to the POTENTIALBEGINCOMMENT state
          if (character == '/') {
            currentState = ScannerState::POTENTIALBEGINCOMMENT;
          } else {
            // For any other character, generate a SINGULARITY lexeme for
            // the character and stay in the IDLE state
            lexemes.push_back(Lexeme( LexemeType::SINGULARITY,
                                      currentString,
                                      currLexemeStartFileLineNo,
                                      currLexemeStartFileCharNo));
            currentString.clear();
          }
        }
      }
    }
    return true;
  } else {
    return false;
  }
}

/// Processes the next character when the current state is IDENTIFIERPROCESSING
bool Scanner::identifierProcessCharacter( const char character,
                                          std::vector<ParserError>& errors,
                                          std::vector<Lexeme>& lexemes) {
  // Check the character is valid, just to ensure that the state machine is
  // safe - don't report the error as this should be done by the main character
  // processing function
  if (isValidCharacter(character)) {
    // If the character is a valid identifier character, stay in the
    // IDENTIFIERPROCESSING state
    if (isCharacterChar(character)) {
      currentString.push_back(character);
    } else {
      // As the character is not an identifier character, generate the
      // IDENTIFIER lexeme and start a new lexeme
      lexemes.push_back(Lexeme( LexemeType::IDENTIFIER,
                                currentString,
                                currLexemeStartFileLineNo,
                                currLexemeStartFileCharNo));
      startNewLexeme();
      // If the character is a ", this is the start of a string lexeme so go to
      // the STRINGPROCESSING state
      if (character == '"') {
        currentState = ScannerState::STRINGPROCESSING;
      } else {
        currentString.push_back(character);
        // If the character is a /, go to the POTENTIALBEGINCOMMENT state
        if (character == '/') {
          currentState = ScannerState::POTENTIALBEGINCOMMENT;
        } else {
          // If the character is a valid whitespace character, go to the
          // WHITESPACEPROCESSING state
          if (isWhitespaceChar(character)) {
            currentState = ScannerState::WHITESPACEPROCESSING;
          } else {
            // For any other character, generate a SINGULARITY lexeme for
            // the character and move to the IDLE state
            lexemes.push_back(Lexeme( LexemeType::SINGULARITY,
                                      currentString,
                                      currLexemeStartFileLineNo,
                                      currLexemeStartFileCharNo));
            currentString.clear();
            currentState = ScannerState::IDLE;
          }
        }
      }
    }
    return true;
  } else {
    return false;
  }
}

/// Processes the next character when the current state is STRINGPROCESSING
bool Scanner::stringProcessCharacter( const char character,
                                      std::vector<ParserError>& errors,
                                      std::vector<Lexeme>& lexemes) {
  if (isStringChar(character)) {
    // If the character is a ", then it is the closing string delimiter so
    // generate the STRING lexeme and go to the IDLE state
    if (character == '"') {
      lexemes.push_back(Lexeme( LexemeType::STRING,
                                currentString,
                                currLexemeStartFileLineNo,
                                currLexemeStartFileCharNo));
      currentString.clear();
      currentState = ScannerState::IDLE;
    } else {
      // For any other character, just add it to the current string
      currentString.push_back(character);
    }
    return true;
  } else {
    // Add an error - the passed character is not a valid character
    // to be present in a string
    errors.push_back(ParserError( addScannerErrorPrefix(
                                      "Invalid character in string - aborted scanning"),
                                  currentFileLine,
                                  currentLineChar));
    // Return false - there is no point in continuing to scan here, the
    // state machine does not define transitions for invalid characters
    return false;
  }
}

/// Processes the next character when the current state is COMMENTPROCESSING
bool Scanner::commentProcessCharacter(const char character,
                                      std::vector<ParserError>& errors,
                                      std::vector<Lexeme>& lexemes) {
  if (isCommentChar(character)) {
    // Given that it is valid, whatever the character is it should be added to
    // the current string
    currentString.push_back(character);
    // If the character is a *, this may be the beginning of the comment end
    // delimiter so progress to the POTENTIALENDCOMMENT state
    if (character == '*') {
      currentState = ScannerState::POTENTIALENDCOMMENT;
    }
    return true;
  } else {
    // Add an error - the passed character is not a valid character
    // to be present in a comment
    errors.push_back(ParserError( addScannerErrorPrefix(
                                      "Invalid character in comment - aborted scanning"),
                                  currentFileLine,
                                  currentLineChar));
    // Return false - there is no point in continuing to scan here, the
    // state machine does not define transitions for invalid characters
    return false;
  }
}

/// Processes the next character when the current state is WHITESPACEPROCESSING
bool Scanner::whitespaceProcessCharacter( const char character,
                                          std::vector<ParserError>& errors,
                                          std::vector<Lexeme>& lexemes) {
  // Check the character is valid, just to ensure that the state machine is
  // safe - don't report the error as this should be done by the main character
  // processing function
  if (isValidCharacter(character)) {
    // If the character is a valid whitespace character, stay in the
    // WHITESPACEPROCESSING state
    if (isWhitespaceChar(character)) {
      currentString.push_back(character);
    } else {
      // As the character is not whitespace, generate the WHITESPACE lexeme and
      // start a new lexeme
      lexemes.push_back(Lexeme( LexemeType::WHITESPACE,
                                currentString,
                                currLexemeStartFileLineNo,
                                currLexemeStartFileCharNo));
      startNewLexeme();
      // If the character is a ", this is the beginning of a STRING lexeme
      // so go to the STRINGPROCESSING state
      if (character == '"') {
        currentState = ScannerState::STRINGPROCESSING;
      } else {
        // As the character is not a ", it should be added to the current
        // string
        currentString.push_back(character);
        // If the character is a /, go to the POTENTIALBEGINCOMMENT state
        if (character == '/') {
          currentState = ScannerState::POTENTIALBEGINCOMMENT;
        } else {
          // If the character is a valid identifier character, go to the
          // IDENTIFIERPROCESSING state
          if (isCharacterChar(character)) {
            currentState = ScannerState::IDENTIFIERPROCESSING;
          } else {
            // For any other character, generate a SINGULARITY lexeme for
            // the character and move to the IDLE state
            lexemes.push_back(Lexeme( LexemeType::SINGULARITY,
                                      currentString,
                                      currLexemeStartFileLineNo,
                                      currLexemeStartFileCharNo));
            currentString.clear();
            currentState = ScannerState::IDLE;
          }
        }
      }
    }
    return true;
  } else {
    return false;
  }
}

/// Processes the next character when the current state is POTENTIALBEGINCOMMENT
bool Scanner::potBeginCommentProcessCharacter(const char character,
                                              std::vector<ParserError>& errors,
                                              std::vector<Lexeme>& lexemes) {
  // Check the character is valid, just to ensure that the state machine is
  // safe - don't report the error as this should be done by the main character
  // processing function
  if (isValidCharacter(character)) {
    // If the character is a *, this is the end of a comment start delimiter so
    // progress to the COMMENTPROCESSING state
    if (character == '*') {
      currentString.push_back(character);
      currentState = ScannerState::COMMENTPROCESSING;
    } else {
      // As the character is not a *, generate a SINGULARITY lexeme for the /
      // character and put the new character into the current string
      lexemes.push_back(Lexeme( LexemeType::SINGULARITY,
                                currentString,
                                currLexemeStartFileLineNo,
                                currLexemeStartFileCharNo));
      startNewLexeme();
      currentString.push_back(character);
      // If the character is a /, then this may be the beginning of a comment
      // start delimiter so stay in the POTENTIALBEGINCOMMENT state
      if (character != '/') {
        // If the character is a ", then this is the beginning of a STRING
        // lexeme, so go to the STRINGPROCESSING state and clear the current
        // string, as STRING lexemes should not contain delimiters
        if (character == '"') {
          currentState = ScannerState::STRINGPROCESSING;
          currentString.clear();
        } else {
          // If the character is a valid identifier character, go to the
          // IDENTIFIERPROCESSING state
          if (isCharacterChar(character)) {
            currentState = ScannerState::IDENTIFIERPROCESSING;
          } else {
            // If the character is a valid whitespace character, go to the
            // WHITESPACEPROCESSING state
            if (isWhitespaceChar(character)) {
              currentState = ScannerState::WHITESPACEPROCESSING;
            } else {
              // For any other character, generate a SINGULARITY lexeme for
              // the character and move to the IDLE state
              lexemes.push_back(Lexeme( LexemeType::SINGULARITY,
                                        currentString,
                                        currLexemeStartFileLineNo,
                                        currLexemeStartFileCharNo));
              currentString.clear();
              currentState = ScannerState::IDLE;
            }
          }
        }
      }
    }
    return true;
  } else {
    return false;
  }
}

/// Processes the next character when the current state is POTENTIALENDCOMMENT
bool Scanner::potEndCommentProcessCharacter(const char character,
                                            std::vector<ParserError>& errors,
                                            std::vector<Lexeme>& lexemes) {
  if (isCommentChar(character)) {
    // Given that it is valid, whatever the character is it should be added to
    // the current string
    currentString.push_back(character);
    // If the character is a *, this may be the beginning of the comment end
    // delimiter so stay in the POTENTIALENDCOMMENT state
    if (character != '*') {
      // If the character is a /, this is the end of the comment end delimiter
      // so progress to the IDLE state and generate the COMMENT lexeme
      if (character == '/') {
        currentState = ScannerState::IDLE;
        lexemes.push_back(Lexeme( LexemeType::COMMENT,
                                  currentString,
                                  currLexemeStartFileLineNo,
                                  currLexemeStartFileCharNo));
        currentString.clear();
      } else {
        // If the character is anything else, return to the COMMENTPROCESSING
        // state, as the * was not the beginning of the comment end delimiter
        currentState = ScannerState::COMMENTPROCESSING;
      }
    }
    return true;
  } else {
    // Add an error - the passed character is not a valid character
    // to be present in a comment
    errors.push_back(ParserError( addScannerErrorPrefix(
                                      "Invalid character in comment - aborted scanning"),
                                  currentFileLine,
                                  currentLineChar));
    // Return false - there is no point in continuing to scan here, the
    // state machine does not define transitions for invalid characters
    return false;
  }
}

/// Processes the end of the file as an input to the scanning state machine
void Scanner::processEndOfFile( std::vector<ParserError>& errors,
                                std::vector<Lexeme>& lexemes) {
  switch (currentState) {
    case ScannerState::IDLE: {
      // Just return - no lexeme to be generated, no error to be added
      break;
    }
    case ScannerState::IDENTIFIERPROCESSING: {
      // Terminate and generate the IDENTIFIER lexeme
      lexemes.push_back(Lexeme( LexemeType::IDENTIFIER,
                                currentString,
                                currLexemeStartFileLineNo,
                                currLexemeStartFileCharNo));
      break;
    }
    case ScannerState::STRINGPROCESSING: {
      // Add an error - string should not be unterminated at the end of a file
      errors.push_back(ParserError( addScannerErrorPrefix(
                                        "String unterminated by end of file"),
                                    currLexemeStartFileLineNo,
                                    currLexemeStartFileCharNo));
      break;
    }
    case ScannerState::POTENTIALENDCOMMENT:
    case ScannerState::COMMENTPROCESSING: {
      // Add an error - comment should not be unterminated at the end of a file
      errors.push_back(ParserError( addScannerErrorPrefix(
                                        "Comment unterminated by end of file"),
                                    currLexemeStartFileLineNo,
                                    currLexemeStartFileCharNo));
      break;
    }
    case ScannerState::WHITESPACEPROCESSING: {
      // Terminate and generate the WHITESPACE lexeme
      lexemes.push_back(Lexeme( LexemeType::WHITESPACE,
                                currentString,
                                currLexemeStartFileLineNo,
                                currLexemeStartFileCharNo));
      break;
    }
    case ScannerState::POTENTIALBEGINCOMMENT: {
      // Terminate and generate a SINGULARITY lexeme
      lexemes.push_back(Lexeme( LexemeType::SINGULARITY,
                                currentString,
                                currLexemeStartFileLineNo,
                                currLexemeStartFileCharNo));
      break;
    }
  }
}

/// Resets the scanning state machine to its start state and the beginning of the file and also clears the working string
void Scanner::resetToStartState() {
  currentState = ScannerState::START;
  currentString.clear();
  currentFileLine = 1;
  currentLineChar = 0;
  currLexemeStartFileLineNo = 0;
  currLexemeStartFileCharNo = 0;
}

/// Appends the string "SCANNER ERROR: " to the front of the string to indicate the module raising the error
std::string Scanner::addScannerErrorPrefix(std::string errorMessage) {
  std::string returnMessage = "SCANNER ERROR: ";
  returnMessage.append(errorMessage);
  return returnMessage;
}

/// Starts a new lexeme by clearing the current string and setting the lexeme start position to the current character
void Scanner::startNewLexeme() {
  currentString.clear();
  currLexemeStartFileLineNo = currentFileLine;
  currLexemeStartFileCharNo = currentLineChar;
}
