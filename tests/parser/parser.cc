#include <catch.h>

#include <istream>
#include <utility>

#include "../../src/parser/parser.h"
#include "../../src/structures/definition.h"

SCENARIO("ParseValue works independantly") {
  WHEN("ParseValue is called with a : delimiter") {
    std::istringstream input("   foo_ident  :  ");
    std::string value = Parser::parseValue(input);
    THEN("The value is read correctly") {
      REQUIRE( value.compare("foo_ident") == 0 );
    }
    AND_THEN("The next character is the : delimiter") {
      REQUIRE( (char)input.get() == ':' );
    }
  }
  WHEN("ParseValue is called with a , delimiter") {
    std::istringstream input("   foo_ident  ,  ");
    std::string value = Parser::parseValue(input);
    THEN("The value is read correctly") {
      REQUIRE( value.compare("foo_ident") == 0 );
    }
    AND_THEN("The next character is the : delimiter") {
      REQUIRE( (char)input.get() == ',' );
    }
  }
  WHEN("ParseValue is called with a } delimiter") {
    std::istringstream input("   foo_ident  }  ");
    std::string value = Parser::parseValue(input);
    THEN("The value is read correctly") {
      REQUIRE( value.compare("foo_ident") == 0 );
    }
    AND_THEN("The next character is the : delimiter") {
      REQUIRE( (char)input.get() == '}' );
    }
  }
  WHEN("ParseValue is called with a no delimiter") {
    std::istringstream input("   foo_ident    ");
    std::string value = Parser::parseValue(input);
    THEN("The value is read correctly") {
      REQUIRE( value.compare("foo_ident") == 0 );
    }
    AND_THEN("The stream eof bit is set") {
      REQUIRE( input.eof() );
    }
  }
  WHEN("parseValue is called on a string with quotes") {
    std::istringstream input("  \"woop, spaces and }:{ \" ");
    std::string value = Parser::parseValue(input);
    THEN("Spaces and delimiters make it into the string") {
      CAPTURE( value );
      REQUIRE( value.compare("woop, spaces and }:{ ")==0 );
    }
  }
}

SCENARIO("ParsePair works independantly") {
  WHEN("parsePair is called on a string : string pair") {
    std::istringstream input(" foo : bar ");
    std::pair<std::string, Definition*> pair = Parser::parsePair(input);
    THEN("The pair is correctly extracted") {
      CAPTURE( pair.first );
      CAPTURE( pair.second->value );
      REQUIRE( pair.first.compare("foo") == 0 );
      REQUIRE( pair.second->value.compare("bar") == 0 );
    }
  }
  WHEN("parsePair is called on a string : dict pair") {
    std::istringstream input(" foo : { nest1 : bar }");
    std::pair<std::string, Definition*> pair = Parser::parsePair(input);
    THEN("The pair is correctly extracted") {
      CAPTURE( pair.first );
      REQUIRE( pair.first.compare("foo") == 0 );
      REQUIRE( pair.second->pairs["nest1"]->value.compare("bar") == 0 );
    }
  }
}

SCENARIO("parseDict works independantly") {
  WHEN("parseDict is called on a flat dict") {
    std::istringstream input(" foo:bar, who:wat }");
    Definition * def = Parser::parseDict(input);
    THEN("The dictionary is correctly extracted") {
      REQUIRE( def->pairs["foo"]->value.compare("bar") == 0 );
      REQUIRE( def->pairs["who"]->value.compare("wat") == 0 );
    }
  }
}

SCENARIO("parseDefinition works on a correctly formatted file") {
  WHEN("parseDefinition is called on test1.def") {
    Definition * def = Parser::parseDefinition("tests/parser/test_files/test1.def");

    THEN("All the values are present and correct") {
      REQUIRE( def->pairs["foo"]->value.compare("foo bar") == 0 );
      REQUIRE( def->pairs["key"]->pairs["subkey"]->value.compare("sub_value") == 0 );
      REQUIRE( def->pairs["key"]->pairs["subkey2"]->value.compare("spaced sub value") == 0 );
      REQUIRE( def->pairs["key"]->pairs["lorem"]->value.compare("ipsum") == 0 );
      REQUIRE( def->pairs["pig"]->value.compare("latin") == 0 );
    }
  }
}
