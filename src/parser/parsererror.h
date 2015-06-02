/*! \file parsererror.h
          Contains the definition of the ParserError class
*/

#ifndef PARSERERROR_H_
#define PARSERERROR_H_

#include <string>
#include <sstream>

/// Contains information about a parser error
/*! Stores the error message associated with a particular parser error and a
    record of the file position at which this error was produced */
class ParserError {
 public:
  ParserError(const std::string errorDesc);

  ParserError(const std::string errorDesc, const unsigned int lineNo,
              const unsigned int charNo);

  std::string getErrorDescription();

  unsigned int getLineNo();
   
  unsigned int getCharNo();

  std::string formatError(void);

 private:
  std::string errorDesc;  ///<  The error message for this error

  unsigned int lineNo;    ///<  The number of the line which produced the error
                          /*!<  If this value is 0, then the error does not
                                have associated positional information. */

  unsigned int charNo;    ///<  The number of the character on the line which produced the error
                          /*!<  If this value is 0, then the error does not
                                have associated positional information. */
};

#endif // PARSERERROR_H_
