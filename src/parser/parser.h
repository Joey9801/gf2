#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <fstream>
#include <cctype>

#include "../structures/definition.h"

namespace Parser {
  Definition * parseDefinition(std::string filepath);

  //Reads a dict from the first character after the opening brace
  //until the closing brace
  Definition * parseDict(std::ifstream& stream);

  //Reads a pair. Returns the stream with the trailing comma/brace intact
  std::pair<std::string, Definition*> parsePair(std::ifstream& stream);

  //Reads an identifier
  std::string parseValue(std::ifstream& stream);

  //Does what it says on the tin
  void skipWhitespace(std::ifstream& stream);
}

#endif
