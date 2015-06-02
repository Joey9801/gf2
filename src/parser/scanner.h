/*! \file scanner.h
          Contains the definition of the Scanner class and the ScannerState enum
          class it depends on.
*/

#ifndef SCANNER_H_
#define SCANNER_H_

#include <string>
#include <vector>
#include <limits>

#include "parsererror.h"
#include "lexeme.h"

/*! An enumeration of all the possible scanner states */
enum class ScannerState : char {
  START = 0,                  /*!<  The start state - an additional name for the
                                    "idle" state */
  IDLE = 0,                   /*!<  The "idle" state - the previous character
                                    extracted terminated a lexeme, and the
                                    scanner is waiting to extract another
                                    character in order to start the next lexeme
                                    */
  IDENTIFIERPROCESSING,       /*!<  The "identifier processing" state - one or
                                    more valid identifier characters have been
                                    extracted but the identifier has not been
                                    terminated yet */
  STRINGPROCESSING,           /*!<  The "string processing" state - a string
                                    delimiter character has been extracted,
                                    along with 0 or more valid string
                                    characters but the string has not been
                                    terminated with a closing delimiter yet */
  COMMENTPROCESSING,          /*!<  The "comment processing" state - an opening
                                    comment delimiting sequence has been
                                    extracted along with 0 or more valid
                                    comment characters but the comment has not
                                    been terminated with a closing delimiter
                                    sequence yet */
  WHITESPACEPROCESSING,       /*!<  The "whitespace processing" state - one or
                                    more valid whitespace characters have been
                                    extracted but the whitespace has not been
                                    terminated yet */
  POTENTIALBEGINCOMMENT,      /*!<  The "potential begin comment" state - the
                                    first character of an opening comment
                                    delimiting sequence has been extracted but
                                    the second character has not yet */
  POTENTIALENDCOMMENT         /*!<  The "potential end comment" state - the
                                    first character of a closing comment
                                    delimiting sequence has been extracted but
                                    the second character has not yet */
};

/// Circuit definition file scanner class
/*! Scans a circuit definition source file and extracts all lexemes from it.
    The class also creates a ParserError object for every syntax error it
    is able to detect. */
class Scanner {
 public:
  Scanner();

  bool scan(                            const std::string filename,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

 private:
  bool processCharacter(                const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  bool idleProcessCharacter(            const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  bool identifierProcessCharacter(      const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  bool stringProcessCharacter(          const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  bool commentProcessCharacter(         const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  bool whitespaceProcessCharacter(      const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  bool potBeginCommentProcessCharacter( const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  bool potEndCommentProcessCharacter(   const char character,
                                        std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  void processEndOfFile(                std::vector<ParserError>& errors,
                                        std::vector<Lexeme>& lexemes);

  void resetToStartState();

  std::string addScannerErrorPrefix(const std::string errorMessage);

  void startNewLexeme();

  ScannerState currentState;              ///<  The current state of the scanner state machine

  std::string currentString;              ///<  The string of characters currently being extracted

  unsigned int currentFileLine;           ///<  The number of the source file line from which the most recent character was scanned

  unsigned int currentLineChar;           ///<  The number (on the line) of the most recently scanned character

  unsigned int currLexemeStartFileLineNo; ///<  The number of the source file line from which the first character of the current lexeme was scanned

  unsigned int currLexemeStartFileCharNo; ///<  The number (on the line it was scanned from) of the first character of the current lexeme
};

#endif // SCANNER_H_
