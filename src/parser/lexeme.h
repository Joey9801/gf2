/*! \file lexeme.h
          Contains the definition of the Lexeme class and the LexemeType enum
          class it depends on.
*/

#ifndef LEXEME_H_
#define LEXEME_H_

#include <string>

/*! An enumeration of all the possible lexeme types */
enum class LexemeType : char {
  IDENTIFIER,           /*!<  An identifier lexeme - should contain the full
                              identifier and no more */
  STRING,               /*!<  A string lexeme - should include the full string
                              excluding delimiters, these should not be
                              included in any lexeme */
  SINGULARITY,          /*!<  A singularity lexeme - lexeme type for any single
                              non-whitespace character not included in another
                              lexeme or used as a delimiter for another lexeme
                              */
  WHITESPACE,           /*!<  A whitespace lexeme - should contain the full
                              whitespace string, so two whitespace lexemes
                              should always be separated by another lexeme type
                              */
  COMMENT               /*!<  A comment lexeme - should contain the full
                              comment, including delimiters, and no more */
};

/// Contains information about a lexeme
/*! Stores the type of a lexeme as well as the original sequence of characters
    which compose it. The class also keeps a record of the file position at
    which this sequence began. */
class Lexeme {
 public:
  Lexeme(LexemeType type, std::string charString, unsigned int startLineNo,
    unsigned int startCharNo);

  std::string getString();

  LexemeType getType();

  unsigned int getStartLineNo();

  unsigned int getStartCharNo();

 private :
  LexemeType type;          ///<  The type of the lexeme

  std::string charString;   ///<  The string of characters the lexeme represents

  unsigned int startLineNo; ///<  The line number where the lexeme string starts in the source file

  unsigned int startCharNo; ///<  The number of the character on the line where the lexeme string starts in the source file
};

#endif // LEXEME_H_
