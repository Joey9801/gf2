#include <catch.h>

#include <istream>
#include <sstream>

#include "../../src/parser/parser.h"
#include "../../src/structures/definition.h"

SCENARIO("All the parser functions work independently") {
  WHEN("skipWhitespace is called with no non-whitespace characters") {
    std::istringstream input("      ");
    THEN("The end of file bit is set") {
      Parser::skipWhitespace(input);
      REQUIRE( input.eof() );
    }
  }

  WHEN("skipWhitespace is called with leading whitespace") {
    std::istringstream input("      abcd");
    THEN("The next character out of the stream is the first non-whitespace character") {
      Parser::skipWhitespace(input);
      REQUIRE( input.get() == 'a' );
    }
  }

  WHEN("skipWhitespace is called with no leading whitespace") {
    std::istringstream input("abcd");
    THEN("The next character out of the the stream is the first character") {
      Parser::skipWhitespace(input);
      REQUIRE( input.get() == 'a' );
    }
  }

  WHEN("ParseValue is called with a : delimiter") {
    std::istringstream input("   foo_ident   :");
    THEN("The value is read correctly") {
      std::string value = Parser::parseValue(input);
      REQUIRE( value.compare("foo_ident") == 0 );
    }
    AND_THEN("The next character is the :") {
      REQUIRE( input.get() == ':' );
    }
  }
  WHEN("ParseValue is called with a , delimiter") {
    std::istringstream input("   foo_ident   ,");
    THEN("The value is read correctly") {
      std::string value = Parser::parseValue(input);
      REQUIRE( value.compare("foo_ident") == 0 );
    }
    AND_THEN("The next character is the ,") {
      REQUIRE( input.get() == ',' );
    }
  }
  WHEN("ParseValue is called with a } delimiter") {
    std::istringstream input("   foo_ident   }");
    THEN("The value is read correctly") {
      std::string value = Parser::parseValue(input);
      REQUIRE( value.compare("foo_ident") == 0 );
    }
    AND_THEN("The next character is the }") {
      REQUIRE( input.get() == '}' );
    }
  }
}
