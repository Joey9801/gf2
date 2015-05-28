/*! \file evaluator.h
          Contains the definition of the Evaluator class and the EvaluatorState
          enum class it depends on.
*/

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

#include <vector>

#include "parsererror.h"
#include "lexeme.h"
#include "token.h"

/*! An enumeration of all the possible evaluator states */
enum class EvaluatorState : char {
  START = 0,                  /*!<  The start state - an additional name for the
                                    "idle" state */
  IDLE = 0,                   /*!<  The "idle" state - the previous character
                                    was whitespace, a comment or generated a
                                    token */
  SINGLEIDENTIFIER,           /*!<  The "single identifier" state - a single
                                    identifier lexeme has been read in, but it
                                    is not yet known whether this is the value
                                    in its entirety */
  MEMBERACCESSEDIDENTIFIER    /*!<  The "member accessed identifier" state -
                                    a single identifier was immediately
                                    followed by a . character, so a second
                                    identifier is expected immediately */
};

/// Circuit definition file evaluator class
/*! Evaluates the lexemes from a circuit definition file in order to produce
    the tokens for the file. The class also creates a ParserError object for
    every syntax error it is able to detect. */
class Evaluator {
 public:
   Evaluator();

   bool evaluate( std::vector<Lexeme>& lexemes, 
                  std::vector<ParserError>& errors,
                  std::vector<Token>& tokens);
 private:
   bool processLexeme(      Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens);

   bool idleProcessLexeme(  Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens);

   bool singleIdentifierProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens);

   bool memberAccessedIdentifierProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens);

   void processEndOfLexemes(std::vector<ParserError>& errors,
                            std::vector<Token>& tokens);

   void resetToStartState();

   std::string addEvaluatorErrorPrefix(std::string errorMessage);

   EvaluatorState currentState;           ///<  The current state of the evaluator state machine

   std::string currentString;             ///<  The string of characters currently being extracted

   unsigned int currTokenStartFileLineNo; ///<  The number of the source file line from which the first character of the current token came

   unsigned int currTokenStartFileCharNo; ///<  The number (on the line it came from) of the first character of the current token
};

#endif // EVALUATOR_H_
