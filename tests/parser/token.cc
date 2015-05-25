#include <limits>
#include <catch.h>

#include "../../src/parser/token.h"

SCENARIO("Create some tokens and check their values") {
  WHEN("Create a value token") {
    Token token(TokenType::VALUE, "identifier1.identifier2", 24, 456);
    REQUIRE(token.getType() == TokenType::VALUE);
    REQUIRE(token.getValue() == "identifier1.identifier2");
    REQUIRE(token.getStartLineNo() == 24);
    REQUIRE(token.getStartCharNo() == 456);
  }
  WHEN("Create an alternative value token") {
    Token token(TokenType::VALUE, "component_Name", 5427354, 65474);
    REQUIRE(token.getType() == TokenType::VALUE);
    REQUIRE(token.getValue() == "component_Name");
    REQUIRE(token.getStartLineNo() == 5427354);
    REQUIRE(token.getStartCharNo() == 65474);
  }
  WHEN("Create a string token") {
    Token token(TokenType::STRING, "string, this is", 1, 1);
    REQUIRE(token.getType() == TokenType::STRING);
    REQUIRE(token.getValue() == "string, this is");
    REQUIRE(token.getStartLineNo() == 1);
    REQUIRE(token.getStartCharNo() == 1);
  }
  WHEN("Create a dictionary opening delimiter token") {
    Token token(TokenType::DICTDELIMOPEN, "{", 99, 546);
    REQUIRE(token.getType() == TokenType::DICTDELIMOPEN);
    REQUIRE(token.getValue() == "{");
    REQUIRE(token.getStartLineNo() == 99);
    REQUIRE(token.getStartCharNo() == 546);
  }
  WHEN("Create a dictionary closing delimiter token") {
    Token token(TokenType::DICTDELIMCLOSE, "}", 777, 888);
    REQUIRE(token.getType() == TokenType::DICTDELIMCLOSE);
    REQUIRE(token.getValue() == "}");
    REQUIRE(token.getStartLineNo() == 777);
    REQUIRE(token.getStartCharNo() == 888);
  }
  WHEN("Create a dictionary separator token") {
    Token token(TokenType::DICTSEPARATOR, ",", std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max());
    REQUIRE(token.getType() == TokenType::DICTSEPARATOR);
    REQUIRE(token.getValue() == ",");
    REQUIRE(token.getStartLineNo() == std::numeric_limits<unsigned int>::max());
    REQUIRE(token.getStartCharNo() == std::numeric_limits<unsigned int>::max());
  }
  WHEN("Create a pair separator token") {
    Token token(TokenType::PAIRSEPARATOR, ":", std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::min());
    REQUIRE(token.getType() == TokenType::PAIRSEPARATOR);
    REQUIRE(token.getValue() == ":");
    REQUIRE(token.getStartLineNo() == std::numeric_limits<unsigned int>::min());
    REQUIRE(token.getStartCharNo() == std::numeric_limits<unsigned int>::min());
  }
}
