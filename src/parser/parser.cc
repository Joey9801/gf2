#include "parser.h"

/// Constructor for the Parser class
Parser::Parser() : lexor() {
}

/// Parses a file and returns a pointer to the resulting definition, alongside errors
Definition* Parser::parse(const std::string filename,
                          std::vector<ParserError>& errors) {
  // Initialise the definition pointer as null, as this should be the
  // error condition for the return value
  Definition* def = 0;

  // Lex the file to obtain its tokens
  std::vector<Token> tokens;
  lexor.lex(filename, errors, tokens);

  if (errors.empty()) {
    if (!tokens.empty()) {
      std::vector<Token>::iterator currentToken = tokens.begin();

      // If the first token is an opening dict delimiter as it should be,
      // then begin to parse the dict
      if (currentToken->getType() == TokenType::DICTDELIMOPEN) {
        // Increment the currentToken iterator and parse the dict
        def = parseDict(tokens, ++currentToken, errors);
        if (currentToken != tokens.end()) {
          // If all there are tokens left over after parsing the dict then there
          // is something wrong - return this error
          errors.push_back(ParserError(addParserErrorPrefix(
              "Additional tokens found after main body dict"),
              currentToken->getStartLineNo(),
              currentToken->getStartCharNo()));
        }
      } else {
        // Failed to find main body dict - return this error
        errors.push_back(ParserError(addParserErrorPrefix(
            "First token is not opening delimiter for entire definition"),
            currentToken->getStartLineNo(),
            currentToken->getStartCharNo()));
      }
    } else {
      // No errors from lexor, but no tokens, so file contains no source code
      // - return this error
      errors.push_back(ParserError(addParserErrorPrefix(
        "File does not contain any source code")));
    }
  }

  // If there are errors then delete the invalid definition and return a pointer
  // to a new, empty Definition object to prevent attempts to access broken
  // information
  if (!errors.empty()) {
    delete def;
    def = new Definition;
  }

  return def;
}

/// Parses a dict and returns a pointer to the resulting definition, alongside errors
Definition* Parser::parseDict(std::vector<Token>& tokens,
                              std::vector<Token>::iterator& currentToken,
                              std::vector<ParserError>& errors) {
  // Create a new dict definition and store the pointer in def.
  Definition* def = new Definition(Dict);
  bool returnEoFError = false;

  // Don't dereference currentToken if it is at the end of the token vector
  if (currentToken != tokens.end()) {
    bool dictFinished = false;
    // Repeatedly try to parse pairs until the dict has been parsed
    while (!dictFinished) {
      // currentToken is valid, so try to parse a pair
      std::pair<std::string, Definition*> pair = parsePair( tokens,
                                                            currentToken,
                                                            errors);
      def->pairs[pair.first] = pair.second;
      if (currentToken != tokens.end()) {
        // If the next token is a closing dict delimiter then the dict is
        // finished, so move the iterator past the delimiter and return.
        dictFinished = (currentToken->getType() == TokenType::DICTDELIMCLOSE);
        // The alternative is that the next token is a dict separator, so
        // either way move past it ready to continue parsing
        ++currentToken;
      } else {
        dictFinished = true;
        returnEoFError = true;
      }
    }
  } else {
    returnEoFError = true;
  }

  if (returnEoFError) {
    // End of file occurred in the middle of a dict definition -
    // return this error
    errors.push_back(ParserError(addParserErrorPrefix(
        "End of file occurs in the middle of a dict definition")));
    delete def;
    def = 0;
  }

  return def;
}

/// Parses a pair and returns a std::pair containing a string and a definition pointer, alongside errors
std::pair<std::string, Definition*> Parser::parsePair(
    std::vector<Token>& tokens,
    std::vector<Token>::iterator& currentToken,
    std::vector<ParserError>& errors) {
  // Initialise the pair as an empty string with a null pointer, as the
  // latter will form the error condition for the return value
  std::pair<std::string, Definition*> pair;
  pair.second = 0;
  pair.first.clear();

  // If the current token is a string or an identifier then this may be a
  // valid pair
  if ((currentToken->getType() == TokenType::STRING)      ||
      (currentToken->getType() == TokenType::IDENTIFIER)) {
    pair.first = currentToken->getValue();
    ++currentToken;
    if (currentToken != tokens.end()) {
      // If the current token is now a pair separator then this may be a valid
      // pair
      if (currentToken->getType() == TokenType::PAIRSEPARATOR) {
        ++currentToken;
        if (currentToken != tokens.end()) {
          bool validPair = true;
          // If the current token is now a string or value then this is a valid
          // pair
          if ((currentToken->getType() == TokenType::STRING)      ||
              (currentToken->getType() == TokenType::IDENTIFIER)  ||
              (currentToken->getType() == TokenType::VALUE))      {
            pair.second = new Definition(currentToken->getValue());
            ++currentToken;
          } else {
            // Otherwise, if the current token is an opening dict delimiter
            // then this may be a valid pair, so attempt to parse the dict
            if (currentToken->getType() == TokenType::DICTDELIMOPEN) {
              pair.second = parseDict(tokens, ++currentToken, errors);
            } else {
              // Not a valid pair - return this error
              errors.push_back(ParserError(addParserErrorPrefix(
                  "String, value or dict must follow pair separator"),
                  currentToken->getStartLineNo(),
                  currentToken->getStartCharNo()));
              validPair = false;
            }
          }
          // Do not return error if the end of the token vector has been
          // reached, the caller will deal with that
          if (currentToken != tokens.end()) {
            if ((currentToken->getType() != TokenType::DICTDELIMCLOSE) &&
                (currentToken->getType() != TokenType::DICTSEPARATOR)) {
              // If the current token is now neither a dict separator or a
              // closing dict delimiter then return an error and scan until
              // it is or the end of the file is reached
              if (validPair) {
                errors.push_back(ParserError(addParserErrorPrefix(
                    "Dict separator or closing dict delimiter must follow pair"),
                    currentToken->getStartLineNo(),
                    currentToken->getStartCharNo()));
              }
              bool found = false;
              while (!found) {
                ++currentToken;
                if (currentToken != tokens.end()) {
                  if ((currentToken->getType() == TokenType::DICTDELIMCLOSE)||
                      (currentToken->getType() == TokenType::DICTSEPARATOR)){
                    found = true;
                  }
                } else {
                  found = true;
                }
              }
            }
          }
        } else {
          // End of file occurred in the middle of a pair definition -
          // return this error
          errors.push_back(ParserError(addParserErrorPrefix(
              "End of file occurs in the middle of a pair definition")));
        }
      } else {
        // Expected pair separator but didn't get one - return this error
        errors.push_back(ParserError(addParserErrorPrefix(
            "Expected pair separator"),
            currentToken->getStartLineNo(),
            currentToken->getStartCharNo()));
      }
    } else {
      // End of file occurred in the middle of a pair definition -
      // return this error
      errors.push_back(ParserError(addParserErrorPrefix(
          "End of file occurs in the middle of a pair definition")));
    }
  } else {
    // Expected string or identifier to start pair but didn't get one - 
    // return this error
    errors.push_back(ParserError(addParserErrorPrefix(
        "Pair must start with an identifier or a string"),
        currentToken->getStartLineNo(),
        currentToken->getStartCharNo()));
  }

  return pair;
}

/// Appends the string "PARSER ERROR: " to the front of the string to indicate the module raising the error
std::string Parser::addParserErrorPrefix(std::string errorMessage) {
  std::string returnMessage = "PARSER ERROR: ";
  returnMessage.append(errorMessage);
  return returnMessage;
}
