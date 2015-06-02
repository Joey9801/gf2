#include "parsererror.h"

/// Constructor for the ParserError class with no positional information
/*! This constructor should be used when the error being created is not
    associated with a position in the source file. */
ParserError::ParserError(const std::string errorDesc)
    : errorDesc(errorDesc),
      lineNo(0),
      charNo(0) {
}

/// Constructor for the ParserError class with positional information
/*! This constructor should be used when the error being created is associated
    with a particular line and character number in the source file. */
ParserError::ParserError( const std::string errorDesc,
                          const unsigned int lineNo, const unsigned int charNo)
    : errorDesc(errorDesc),
      lineNo(lineNo),
      charNo(charNo) {
}

/// Returns the error message associated with this error.
std::string ParserError::getErrorDescription() {
  return errorDesc;
}

/// Returns the number of the line which produced the error.
/*! If the value returned is 0, then the error does not have associated
    positional information. */
unsigned int ParserError::getLineNo() {
  return lineNo;
}

/// Returns the number of the character on the line which produced the error.
/*! If the value returned is 0, then the error does not have associated
    positional information. */
unsigned int ParserError::getCharNo() {
  return charNo;
}