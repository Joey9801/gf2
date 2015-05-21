#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <cctype>

#include "../structures/definition.h"

namespace Parser {
  const std::vector<char> delimiters = {'{', '}', ':', ','};
  Definition * parseDefinition(std::string filepath);

  //Reads a dict from the first character after the opening brace
  //until the closing brace
  Definition * parseDict(std::istream& stream);

  //Reads a pair. Returns the stream with the trailing comma/brace intact
  std::pair<std::string, Definition*> parsePair(std::istream& stream);

  //Reads an identifier
  std::string parseValue(std::istream& stream);

  bool isdelim(char c);
}

#endif
