#include "parser.h"

namespace Parser {
  Definition * parseDefinition(std::string filepath) {
    Definition * def = new Definition();

    //Opens a filestream

    return def;
  }

  Definition * parseDict(std::ifstream& stream) {
    Definition * def = new Definition(Dict);

    //std::pair<std::string, Definition*> pair;
    //while(not the end of the dict) {
    //  pair = parsePair(stream);
    //  def->pairs[pair.first] = pair.second;
    //}
    return def;
  }

  std::pair<std::string, Definition*> parsePair(std::ifstream& stream) {
    std::pair<std::string, Definition*> pair;

    skipWhitespace(stream);
    pair.first = parseValue(stream);
    skipWhitespace(stream);
    if(stream.get() == '{')
      pair.second = parseDict(stream);
    else
      pair.second = new Definition(parseValue(stream));


    return pair;
  }

  std::string parseValue(std::ifstream& stream) {
    //Reads a string in until one of the following characters are met:
    //   } : ,
    //If the first character is a quote, spaces are allowed.

    std::string s;
    s.reserve(100); //Bound to be enough for most things

    //Does some magic

    return s;
  }

  void skipWhitespace(std::ifstream& stream) {
    while( isspace(stream.get()) );
    stream.unget();
  }

}
