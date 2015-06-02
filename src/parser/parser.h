/*! \file parser.h
          Contains the definition of the Parser class.
*/

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

#include "lexor.h"
#include "../structures/definition.h"
#include "../errors/error.h"

/// Circuit definition file parser class
/*! Uses a lexor to obtain all the tokens from a source file, and then parses
    these to produce a circuit definition. */
class Parser {
 public:
  Parser();

  Definition* parse(    const std::string filename,
                        std::vector<ParserError>& errors);

  Definition* parseDict(std::vector<Token>& tokens,
                        std::vector<Token>::iterator& currentToken,
                        std::vector<ParserError>& errors);

  std::pair<std::string, Definition*> parsePair(
                        std::vector<Token>& tokens,
                        std::vector<Token>::iterator& currentToken,
                        std::vector<ParserError>& errors);

 private:
  std::string addParserErrorPrefix(const std::string errorMessage);

  Lexor lexor;  ///<  The parser's internal lexor object
};

#endif // PARSER_H_
