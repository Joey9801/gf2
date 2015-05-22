#include "parser.h"

namespace Parser {
  Definition * parseDefinition(std::string filepath) {
    Definition * def;

    //Opens a filestream
    std::ifstream stream(filepath);
    if( not stream.good() ) {
      std::cerr << "failed to open file" << std::endl;
      throw 1;
    }

    char c;
    stream.setf(std::ios_base::skipws);
    stream >> c;
    if( c=='{' )
      def = parseDict(stream);
    else {
      //Don't panic Pike
      std::cerr << "Initial { not found" << std::endl;
      throw 1;
    }

    return def;
  }

  Definition * parseDict(std::istream& stream) {
    Definition * def = new Definition(Dict);

    stream.setf(std::ios_base::skipws);
    std::pair<std::string, Definition*> pair;
    char c;

    while( true ) {
      pair = parsePair(stream);
      def->pairs[pair.first] = pair.second;

      stream >> c;
      if( (c == '}') or stream.eof() )
        break;
      else if (c == ',')
        continue;
      else {
        //Panic, it's all gone to shits
        std::cerr << "not a , or the end of a dict D:" << std::endl;
        throw 1;
      }
    }
    return def;
  }

  std::pair<std::string, Definition*> parsePair(std::istream& stream) {

    stream.setf(std::ios_base::skipws);
    char c;
    std::pair<std::string, Definition*> pair;

    pair.first = parseValue(stream);
    stream >> c;
    if(c != ':') {
      //panic, everything's going wrong
      std::cerr << "no colon in a pair" << std::endl;
      throw 1;
    }

    stream >> c;
    if(c == '{') {
      pair.second = parseDict(stream);
    }
    else {
      stream.unget();
      pair.second = new Definition(parseValue(stream));
    }

    return pair;
  }

  std::string parseValue(std::istream& stream) {
    std::string s;

    char c;
    stream >> c;

    if( c=='"' ) {
      c = stream.get();
      while( c!='"' ) {
        s += c;
        c = stream.get();
        if( stream.eof() ) {
          //Panic, it's all gone to shits
          std::cerr << "Reached end of file while reading string" << std::endl;
          throw 1;
        }
      }
    }
    else {
      stream.setf(std::ios_base::skipws);
      while( not isdelim(c) ) {
        s += c;
        stream >> c;
        if( stream.eof() )
          return s;
      }
    stream.unget();
    }

    return s;
  }

  bool isdelim(char c) {
    for(unsigned int i=0; i<delimiters.size(); i++)
      if( c==delimiters[i] )
        return true;
    return false;
  }
}
