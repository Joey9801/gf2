/*! \file lexor.h
          Contains the definition of the Lexor class.
*/

#ifndef LEXOR_H_
#define LEXOR_H_

#include <string>
#include <vector>

#include "parsererror.h"
#include "token.h"
#include "scanner.h"
#include "evaluator.h"

/// Circuit definition file lexor class
/*! Uses a scanner to obtain all the lexemes in a source file, and then passes
    these to an evaluator in order to obtain the tokens. */
class Lexor {
 public:
   Lexor();

   bool lex(const std::string filename,
            std::vector<ParserError>& errors,
            std::vector<Token>& tokens);

 private:
   Scanner scanner;     ///<  The lexor's internal scanner object

   Evaluator evaluator; ///<  The lexor's internal evaluator object
};

#endif // LEXOR_H_
