/*! \file token.h
          Contains the definition of the Token class and the TokenType enum
          class it depends on.
*/

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

/*! An enumeration of all the possible token types */
enum class TokenType : char {
  IDENTIFIER,           /*!<  An identifier token - should contain the full
                              identifier and no more */
  VALUE,                /*!<  A value token - should contain the full value and
                              no more. This is a value in the context of the
                              syntax of the circuit definition language */
  STRING,               /*!<  A string token - should include the full string
                              excluding delimiters, these should not be
                              included in any token */
  DICTDELIMOPEN,        /*!<  An opening dict delimiter token - should be just
                              the dict delimiting character */
  DICTDELIMCLOSE,       /*!<  A closing dict delimiter token - should be just
                              the dict delimiting character */
  DICTSEPARATOR,        /*!<  A dict separator token - should be just the
                              dict separator character */
  PAIRSEPARATOR         /*!<  A pair separator token - should be just the
                              pair separator character */
};

/// Contains information about a token
/*! Stores the type of a token as well as the original sequence of characters
    which compose it. The class also keeps a record of the file position at
    which this sequence began. The use of a string to store the value for all
    tokens does not strictly conform to the computer science concept of a token
    but prevents conversion back to a string to conform to the constraints of
    the Definition class. */
class Token {
 public:
  Token(const TokenType type, const std::string value,
        const unsigned long long int startLineNo,
        const unsigned long long int startCharNo);

  std::string getValue();

  TokenType getType();

  unsigned long long int getStartLineNo();

  unsigned long long int getStartCharNo();

 private :
  TokenType type;                     ///<  The type of the token

  std::string value;                  ///<  The string of characters the token represents
                                      /*!<  Note that this is a value in the context of a
                                            token in computer science, not as defined in
                                            the circuit definition language (although the
                                            two definitions are identical for an
                                            identifier) */

  unsigned long long int startLineNo; ///<  The line number where the token string starts in the source file

  unsigned long long int startCharNo; ///<  The number of the character on the line where the token string starts in the source file
};

#endif // TOKEN_H_
