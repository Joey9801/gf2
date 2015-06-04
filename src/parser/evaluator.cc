#include "evaluator.h"

#include "charcheck.h"

/// Constructor for the Evaluator class
Evaluator::Evaluator() : currentState(EvaluatorState::START) {
  resetToStartState();
}

/// Evaluates lexemes and returns a vector of the tokens it produces
bool Evaluator::evaluate( std::vector<Lexeme>& lexemes,
                          std::vector<ParserError>& errors,
                          std::vector<Token>& tokens) {
  resetToStartState();
  // Clear the vector of tokens to prevent mixing old and new tokens
  tokens.clear();

  bool aborted = false;
  for ( std::vector<Lexeme>::iterator lexeme = lexemes.begin();
        (lexeme != lexemes.end() && !aborted);
        ++lexeme) {
    // Sequentially process all lexemes in the vector
    aborted = !processLexeme(*lexeme, errors, tokens);
  }
  if (!aborted) {
    // Process the end of the vector of lexemes
    processEndOfLexemes(errors, tokens);
  }

  // If there are errors then clear the vector of tokens to prevent use of
  // a half-complete vector
  if (!errors.empty()) {
    tokens.clear();
  }

  return errors.empty();
}

/// Processes the next lexeme as an input to the evaluator state machine
bool Evaluator::processLexeme(Lexeme& lexeme,
                    std::vector<ParserError>& errors,
                    std::vector<Token>& tokens) {
  switch (currentState) {
    case EvaluatorState::IDLE: {
      return idleProcessLexeme(lexeme, errors, tokens);
    }
    case EvaluatorState::SINGLEIDENTIFIER: {
      return singleIdentifierProcessLexeme(lexeme, errors, tokens);
    }
    case EvaluatorState::MEMBERACCESSEDIDENTIFIER: {
      return memberAccessedIdentifierProcessLexeme(lexeme, errors, tokens);
    }
    case EvaluatorState::VALUE: {
      return valueProcessLexeme(lexeme, errors, tokens);
    }
    case EvaluatorState::VALUEVECTOROPEN: {
      return valueVectorOpenProcessLexeme(lexeme, errors, tokens);
    }
    case EvaluatorState::VALUEVECTORNUMBERED: {
      return valueVectorNumberedProcessLexeme(lexeme, errors, tokens);
    }
    case EvaluatorState::IDENTIFIERVECTOROPEN: {
      return identifierVectorOpenProcessLexeme(lexeme, errors, tokens);
    }
    case EvaluatorState::IDENTIFIERVECTORNUMBERED: {
      return identifierVectorNumberedProcessLexeme(lexeme, errors, tokens);
    }
  }
  // Return false, as something has gone wrong with currentState
  return false;
}

/// Processes the next lexeme when the current state is IDLE
bool Evaluator::idleProcessLexeme(Lexeme& lexeme,
                                  std::vector<ParserError>& errors,
                                  std::vector<Token>& tokens) {
  // If the lexeme is WHITESPACE or COMMENT then just stay in the same state
  // by skipping all other decisions and returning true
  if (!((lexeme.getType() == LexemeType::WHITESPACE) ||
        (lexeme.getType() == LexemeType::COMMENT))) {
    if (lexeme.getType() == LexemeType::STRING) {
      // If the lexeme is a string then generate a STRING token from it
      tokens.push_back(Token( TokenType::STRING,
                              lexeme.getString(),
                              lexeme.getStartLineNo(),
                              lexeme.getStartCharNo()));
    } else {
      if (lexeme.getType() == LexemeType::IDENTIFIER) {
        // If the lexeme is an identifier then go to the SINGLEIDENTIFIER state
        // and set up evaluator's internal information about the value being
        // formed
        currentString = lexeme.getString();
        currTokenStartFileLineNo = lexeme.getStartLineNo();
        currTokenStartFileCharNo = lexeme.getStartCharNo();
        currentState = EvaluatorState::SINGLEIDENTIFIER;
      } else {
        if ((lexeme.getType() == LexemeType::SINGULARITY) && 
            (lexeme.getString().length() == 1)) {
          if (isOpeningDictDelim(lexeme.getString()[0])) {
            // If the lexeme is an opening dict delimiter then generate a
            // DICTDELIMOPEN token from it
            tokens.push_back(Token( TokenType::DICTDELIMOPEN,
                                    lexeme.getString(),
                                    lexeme.getStartLineNo(),
                                    lexeme.getStartCharNo()));
          } else {
            if (isClosingDictDelim(lexeme.getString()[0])) {
              // If the lexeme is a closing dict delimiter then generate a
              // DICTDELIMCLOSE token from it
              tokens.push_back(Token( TokenType::DICTDELIMCLOSE,
                                      lexeme.getString(),
                                      lexeme.getStartLineNo(),
                                      lexeme.getStartCharNo()));
            } else {
              if (isDictSeparator(lexeme.getString()[0])) {
                // If the lexeme is a dict separator then generate a
                // DICTSEPARATOR token from it
                tokens.push_back(Token( TokenType::DICTSEPARATOR,
                                        lexeme.getString(),
                                        lexeme.getStartLineNo(),
                                        lexeme.getStartCharNo()));
              } else {
                if (isPairSeparator(lexeme.getString()[0])) {
                  // If the lexeme is a pair separator then generate a
                  // PAIRSEPARATOR token from it
                  tokens.push_back(Token( TokenType::PAIRSEPARATOR,
                                          lexeme.getString(),
                                          lexeme.getStartLineNo(),
                                          lexeme.getStartCharNo()));
                } else {
                  // If the lexeme is any other singularity then generate
                  // an error and abort evaluation
                  errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                                    "Unexpected character - possible mis-typed delimiter or separator - aborting evaluation"),
                                                lexeme.getStartLineNo(),
                                                lexeme.getStartCharNo()));
                  return false;
                }
              }
            }
          }
        } else {
          // Lexeme should definitely be a SINGULARITY lexeme with a string
          // length of 1, so return false as something has gone wrong if
          // control reaches this statement
          if (lexeme.getType() == LexemeType::SINGULARITY) {
            errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                              "Unexpected singularity length - aborting evaluation"),
                                          lexeme.getStartLineNo(),
                                          lexeme.getStartCharNo()));
          } else {
            errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                              "Unexpected lexeme type - aborting evaluation"),
                                          lexeme.getStartLineNo(),
                                          lexeme.getStartCharNo()));
          }
          return false;
        }
      }
    }
  }
  return true;
}

/// Processes the next lexeme when the current state is SINGLEIDENTIFIER
bool Evaluator::singleIdentifierProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens) {
  if ((lexeme.getType() == LexemeType::WHITESPACE)  ||
      (lexeme.getType() == LexemeType::COMMENT)     ||
      (lexeme.getType() == LexemeType::STRING)) {
    // If the lexeme is a WHITESPACE, COMMENT or STRING lexeme then generate a
    // IDENTIFIER token and go to the IDLE state
    // If the lexeme is a STRING lexeme then also generate a STRING token
    tokens.push_back(Token( TokenType::IDENTIFIER,
                            currentString,
                            currTokenStartFileLineNo,
                            currTokenStartFileCharNo));
    currentString.clear();
    if (lexeme.getType() == LexemeType::STRING) {
      tokens.push_back(Token( TokenType::STRING,
                              lexeme.getString(),
                              lexeme.getStartLineNo(),
                              lexeme.getStartCharNo()));
    }
    currentState = EvaluatorState::IDLE;
  } else {
    if (lexeme.getType() == LexemeType::IDENTIFIER) {
      // If the lexeme is an IDENTIFIER lexeme then something has gone wrong
      // as it should be impossible for the scanner to produce two consecutive
      // IDENTIFIER lexemes so generate an error and return false as the next
      // state is undefined
      errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                        "Two consecutive IDENTIFIER lexemes received - aborting evaluation"),
                                    lexeme.getStartLineNo(),
                                    lexeme.getStartCharNo()));
      return false;
    } else {
      if ((lexeme.getType() == LexemeType::SINGULARITY) && 
          (lexeme.getString().length() == 1)) {
        char singularityChar = lexeme.getString()[0];
        if (isOpeningVectorBracket(singularityChar)) {
          // If the lexeme is an opening vector bracket then append the lexeme
          // to the current string and go to the IDENTIFIERVECTOROPEN state
          currentState = EvaluatorState::IDENTIFIERVECTOROPEN;
          currentString.append(lexeme.getString());
        } else {
          if (isOpeningDictDelim(singularityChar) ||
              isClosingDictDelim(singularityChar) ||
              isDictSeparator(singularityChar)    ||
              isPairSeparator(singularityChar)) {
            // Generate the IDENTIFIER token for the previous lexeme
            tokens.push_back(Token( TokenType::IDENTIFIER,
                                    currentString,
                                    currTokenStartFileLineNo,
                                    currTokenStartFileCharNo));
            currentString.clear();
            // Set type to VALUE so that it can be tested for change
            TokenType type = TokenType::VALUE;
            if (isOpeningDictDelim(singularityChar)) {
              type = TokenType::DICTDELIMOPEN;
            } else {
              if (isClosingDictDelim(singularityChar)) {
                type = TokenType::DICTDELIMCLOSE;
              } else {
                if (isDictSeparator(singularityChar)) {
                  type = TokenType::DICTSEPARATOR;
                } else {
                  if (isPairSeparator(singularityChar)) {
                    type = TokenType::PAIRSEPARATOR;
                  }
                }
              }
            }
            // If type has been changed then create token, otherwise generate an
            // error and abort evaluation
            if (type == TokenType::VALUE) {
              // Control should never reach here, but it is here for safety
              // just in case
              errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                                "A SINGULARITY lexeme with mismatched string and type was sent - aborting evaluation"),
                                            lexeme.getStartLineNo(),
                                            lexeme.getStartCharNo()));
              return false;
            } else {
              tokens.push_back(Token( type,
                                      lexeme.getString(),
                                      lexeme.getStartLineNo(),
                                      lexeme.getStartCharNo()));
              currentState = EvaluatorState::IDLE;
            }
          } else {
            if (isMemberAccessOperator(singularityChar)) {
              // If the lexeme is the member access operator then append it to
              // the current string and go to the MEMBERACCESSEDIDENTIFIER state
              currentString.append(lexeme.getString());
              currentState = EvaluatorState::MEMBERACCESSEDIDENTIFIER;
            } else {
              // If the lexeme is any other singularity then generate an error
              // and abort evaluation as the next state is undefined
              errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                                "Unexpected character - possible mis-typed delimiter, separator, member access operator or identifier - aborting evaluation"),
                                            lexeme.getStartLineNo(),
                                            lexeme.getStartCharNo()));
              return false;
            }
          }
        }
      } else {
        // Lexeme should definitely be a SINGULARITY lexeme with a string
        // length of 1, so return false as something has gone wrong if
        // control reaches this statement
        if (lexeme.getType() == LexemeType::SINGULARITY) {
          errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                            "Unexpected singularity length - aborting evaluation"),
                                        lexeme.getStartLineNo(),
                                        lexeme.getStartCharNo()));
        } else {
          errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                            "Unexpected lexeme type - aborting evaluation"),
                                        lexeme.getStartLineNo(),
                                        lexeme.getStartCharNo()));
        }
        return false;
      }
    }
  }
  return true;
}

/// Processes the next lexeme when the current state is MEMBERACCESSEDIDENTIFIER
bool Evaluator::memberAccessedIdentifierProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens) {
  if (lexeme.getType() == LexemeType::IDENTIFIER) {
    // If the lexeme is an IDENTIFIER lexeme then append the identifier to the
    // current string and go to the VALUE state
    currentString.append(lexeme.getString());
    currentState = EvaluatorState::VALUE;
    return true;
  } else {
    // Add an error - the passed lexeme is not an IDENTIFIER lexeme but should
    // be as it is after a member access operator
    errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                      "Expected identifier after member access operator - aborting evaluation"),
                                  lexeme.getStartLineNo(),
                                  lexeme.getStartCharNo()));
    // Return false - there is no point in continuing to evaluate here as there
    // is no way to work out which state the evaluator should be in
    return false;
  }
}

bool Evaluator::valueProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens) {
  if (lexeme.getType() == LexemeType::SINGULARITY) {
    if (lexeme.getString().length() == 1) {
      if (isOpeningVectorBracket(lexeme.getString()[0])) {
        // If the lexeme is an opening vector bracket then append the lexeme
        // to the current string and go to the VALUEVECTOROPEN state
        currentState = EvaluatorState::VALUEVECTOROPEN;
        currentString.append(lexeme.getString());
        return true;
      }
    }
  }
  // If it is anything else then generate the VALUE token and move to the
  // IDLE state, then re-input this lexeme (This is a shortcut, as other
  // than the opening vector bracket this state should respond identically
  // to the IDLE state)
  tokens.push_back(Token( TokenType::VALUE,
                          currentString,
                          currTokenStartFileLineNo,
                          currTokenStartFileCharNo));
  currentString.clear();
  currentState = EvaluatorState::IDLE;
  return idleProcessLexeme(lexeme, errors, tokens);
}

bool Evaluator::valueVectorOpenProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens) {
  if (lexeme.getType() == LexemeType::IDENTIFIER) {
    // If the lexeme is an identifier then check if it is a valid vector number
    if (isValidVectorNumber(lexeme.getString())) {
      // If it is a valid vector number then append it to the current string and
      // go to the VALUEVECTORNUMBERED state
      currentString.append(lexeme.getString());
      currentState = EvaluatorState::VALUEVECTORNUMBERED;
      return true;
    }
    // Otherwise allow drop-out to produce error
  } else {
    if (lexeme.getType() == LexemeType::SINGULARITY) {
      if (lexeme.getString().length() == 1) {
        if (isClosingVectorBracket(lexeme.getString()[0])) {
          // If the lexeme is a closing vector bracket then append the lexeme
          // to the current string, generate the VALUE token and go to the
          // IDLE state
          currentString.append(lexeme.getString());
          tokens.push_back(Token( TokenType::VALUE,
                                  currentString,
                                  currTokenStartFileLineNo,
                                  currTokenStartFileCharNo));
          currentString.clear();
          currentState = EvaluatorState::IDLE;
          return true;
        }
      }
    }
  }
  // If it is not a valid vector number or a closing vector bracket, produce
  // an error
  errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                    "Expected number or closing vector bracket - aborting evaluation"),
                                lexeme.getStartLineNo(),
                                lexeme.getStartCharNo()));
  return false;
}

bool Evaluator::valueVectorNumberedProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens) {
  if (lexeme.getType() == LexemeType::SINGULARITY) {
    if (lexeme.getString().length() == 1) {
      if (isClosingVectorBracket(lexeme.getString()[0])) {
        // If the lexeme is a closing vector bracket then append the lexeme
        // to the current string, generate the VALUE token and go to the
        // IDLE state
        currentString.append(lexeme.getString());
        tokens.push_back(Token( TokenType::VALUE,
                                currentString,
                                currTokenStartFileLineNo,
                                currTokenStartFileCharNo));
        currentString.clear();
        currentState = EvaluatorState::IDLE;
        return true;
      }
    }
  }
  // If it is not a closing vector bracket, produce an error
  errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                    "Expected number or closing vector bracket - aborting evaluation"),
                                lexeme.getStartLineNo(),
                                lexeme.getStartCharNo()));
  return false;
}

bool Evaluator::identifierVectorOpenProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens) {
  if (lexeme.getType() == LexemeType::IDENTIFIER) {
    // If the lexeme is an identifier then check if it is a valid vector number
    if (isValidVectorNumber(lexeme.getString())) {
      // If it is a valid vector number then append it to the current string and
      // go to the IDENTIFIERVECTORNUMBERED state
      currentString.append(lexeme.getString());
      currentState = EvaluatorState::IDENTIFIERVECTORNUMBERED;
      return true;
    }
    // Otherwise allow drop-out to produce error
  } else {
    if (lexeme.getType() == LexemeType::SINGULARITY) {
      if (lexeme.getString().length() == 1) {
        if (isClosingVectorBracket(lexeme.getString()[0])) {
          // If the lexeme is a closing vector bracket then append the lexeme
          // to the current string, generate the IDENTIFIER token and go to the
          // IDLE state
          currentString.append(lexeme.getString());
          tokens.push_back(Token( TokenType::IDENTIFIER,
                                  currentString,
                                  currTokenStartFileLineNo,
                                  currTokenStartFileCharNo));
          currentString.clear();
          currentState = EvaluatorState::IDLE;
          return true;
        }
      }
    }
  }
  // If it is not a valid vector number or a closing vector bracket, produce
  // an error
  errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                    "Expected number or closing vector bracket - aborting evaluation"),
                                lexeme.getStartLineNo(),
                                lexeme.getStartCharNo()));
  return false;
}

bool Evaluator::identifierVectorNumberedProcessLexeme(
                            Lexeme& lexeme,
                            std::vector<ParserError>& errors,
                            std::vector<Token>& tokens) {
  if (lexeme.getType() == LexemeType::SINGULARITY) {
    if (lexeme.getString().length() == 1) {
      if (isClosingVectorBracket(lexeme.getString()[0])) {
        // If the lexeme is a closing vector bracket then append the lexeme
        // to the current string, generate the IDENTIFIER token and go to the
        // IDLE state
        currentString.append(lexeme.getString());
        tokens.push_back(Token( TokenType::IDENTIFIER,
                                currentString,
                                currTokenStartFileLineNo,
                                currTokenStartFileCharNo));
        currentString.clear();
        currentState = EvaluatorState::IDLE;
        return true;
      }
    }
  }
  // If it is not a closing vector bracket, produce an error
  errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                    "Expected number or closing vector bracket - aborting evaluation"),
                                lexeme.getStartLineNo(),
                                lexeme.getStartCharNo()));
  return false;
}

/// Processes the end of the lexemes as an input to the evaluator state machine
void Evaluator::processEndOfLexemes(std::vector<ParserError>& errors,
                                    std::vector<Token>& tokens) {
  switch (currentState) {
    case EvaluatorState::IDLE: {
      // Just return - no token to be generated, no error to be added
      break;
    }
    case EvaluatorState::SINGLEIDENTIFIER: {
      // Generate the VALUE token from the identifier
      tokens.push_back(Token( TokenType::VALUE,
                              currentString,
                              currTokenStartFileLineNo,
                              currTokenStartFileCharNo));
      break;
    }
    case EvaluatorState::MEMBERACCESSEDIDENTIFIER: {
      // Add an error - should have an identifier (at least) before the end
      // of the file
      errors.push_back(ParserError( addEvaluatorErrorPrefix(
                                        "Premature end of file - expected identifier after member access operator")));
      break;
    }
    case EvaluatorState::VALUE: {
      // Generate the VALUE token from the value
      tokens.push_back(Token( TokenType::VALUE,
                              currentString,
                              currTokenStartFileLineNo,
                              currTokenStartFileCharNo));
      break;
    }
    case EvaluatorState::VALUEVECTOROPEN:
    case EvaluatorState::VALUEVECTORNUMBERED:
    case EvaluatorState::IDENTIFIERVECTOROPEN:
    case EvaluatorState::IDENTIFIERVECTORNUMBERED: {
      // Add an error - should have a vector closing bracket (at least) before
      // the end of the file
      errors.push_back(ParserError(addEvaluatorErrorPrefix(
        "Premature end of file - expected vector closing bracket")));
      break;
    }
  }
}

/// Resets the evaluator state machine to its start state clears the working string
void Evaluator::resetToStartState() {
  currentState = EvaluatorState::START;
  currentString.clear();
  currTokenStartFileLineNo = 0;
  currTokenStartFileCharNo = 0;
}

/// Checks if the string is a valid vector number (all digits, nothing else)
bool Evaluator::isValidVectorNumber(std::string str) {
  if (str.empty()) {
    return false;
  }
  for (std::string::iterator itr = str.begin(); itr != str.end(); ++itr) {
    if (!isdigit(*itr)) {
      return false;
    }
  }
  return true;
}

/// Appends the string "EVALUATOR ERROR: " to the front of the string to indicate the module raising the error
std::string Evaluator::addEvaluatorErrorPrefix(std::string errorMessage) {
  std::string returnMessage = "EVALUATOR ERROR: ";
  returnMessage.append(errorMessage);
  return returnMessage;
}
