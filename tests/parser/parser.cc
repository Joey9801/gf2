#include <catch.h>

#include "../../src/parser/parser.h"
#include "../../src/structures/definition.h"

/// Test whether ParsePair works as expected
SCENARIO("Check whether ParsePair works independently of ParseDict") {
  Parser parser;

  std::vector<Token> tokens;
  std::vector<ParserError> errors;
  std::pair<std::string, Definition*> pair;
  std::vector<Token>::iterator tokenItr;

  // Test whether ParsePair works as expected when the pair does not
  // contain a dict
  GIVEN("Pair does not contain a dict") {
    GIVEN("Pair starts with an identifier") {
      tokens.push_back(Token(TokenType::IDENTIFIER, "identifier1", 1, 1));
      tokens.push_back(Token(TokenType::PAIRSEPARATOR, ":", 1, 12));

      WHEN("Second part is an identifier") {
        tokens.push_back(Token(TokenType::IDENTIFIER, "identifier2", 1, 13));

        tokenItr = tokens.begin();

        THEN("ParsePair should return this pair successfully") {
          pair = parser.parsePair(tokens, tokenItr, errors);

          CHECK(errors.empty());
          CHECK(pair.first == "identifier1");
          REQUIRE(pair.second != 0);
          CHECK(pair.second->value == "identifier2");
          CHECK(pair.second->pairs.empty());
          CHECK(pair.second->type == FieldType::Value);
        }
      }

      WHEN("Second part is a value") {
        tokens.push_back(Token(TokenType::IDENTIFIER, "va.lue", 1, 13));

        tokenItr = tokens.begin();

        THEN("ParsePair should return this pair successfully") {
          pair = parser.parsePair(tokens, tokenItr, errors);

          CHECK(errors.empty());
          CHECK(pair.first == "identifier1");
          REQUIRE(pair.second != 0);
          CHECK(pair.second->value == "va.lue");
          CHECK(pair.second->pairs.empty());
          CHECK(pair.second->type == FieldType::Value);
        }
      }

      WHEN("Second part is a string") {
        tokens.push_back(Token(TokenType::IDENTIFIER, "a string", 1, 13));

        tokenItr = tokens.begin();

        THEN("ParsePair should return this pair successfully") {
          pair = parser.parsePair(tokens, tokenItr, errors);

          CHECK(errors.empty());
          CHECK(pair.first == "identifier1");
          REQUIRE(pair.second != 0);
          CHECK(pair.second->value == "a string");
          CHECK(pair.second->pairs.empty());
          CHECK(pair.second->type == FieldType::Value);
        }
      }
    }

    GIVEN("Pair starts with a string") {
      tokens.push_back(Token(TokenType::STRING, "a string 1", 1, 1));
      tokens.push_back(Token(TokenType::PAIRSEPARATOR, ":", 1, 11));

      tokenItr = tokens.begin();

      WHEN("Second part is an identifier") {
        tokens.push_back(Token(TokenType::IDENTIFIER, "identifier2", 1, 12));

        tokenItr = tokens.begin();

        THEN("ParsePair should return this pair successfully") {
          pair = parser.parsePair(tokens, tokenItr, errors);

          CHECK(errors.empty());
          CHECK(pair.first == "a string 1");
          REQUIRE(pair.second != 0);
          CHECK(pair.second->value == "identifier2");
          CHECK(pair.second->pairs.empty());
          CHECK(pair.second->type == FieldType::Value);
        }
      }

      WHEN("Second part is a value") {
        tokens.push_back(Token(TokenType::IDENTIFIER, "va.lue", 1, 12));

        tokenItr = tokens.begin();

        THEN("ParsePair should return this pair successfully") {
          pair = parser.parsePair(tokens, tokenItr, errors);

          CHECK(errors.empty());
          CHECK(pair.first == "a string 1");
          REQUIRE(pair.second != 0);
          CHECK(pair.second->value == "va.lue");
          CHECK(pair.second->pairs.empty());
          CHECK(pair.second->type == FieldType::Value);
        }
      }

      WHEN("Second part is a string") {
        tokens.push_back(Token(TokenType::IDENTIFIER, "a string 2", 1, 12));

        tokenItr = tokens.begin();

        THEN("ParsePair should return this pair successfully") {
          pair = parser.parsePair(tokens, tokenItr, errors);

          CHECK(errors.empty());
          CHECK(pair.first == "a string 1");
          REQUIRE(pair.second != 0);
          CHECK(pair.second->value == "a string 2");
          CHECK(pair.second->pairs.empty());
          CHECK(pair.second->type == FieldType::Value);
        }
      }
    }
  }
}

/// Test whether ParseDict works as expected
SCENARIO("Check whether ParseDict works") {
  Parser parser;

  std::vector<Token> tokens;
  std::vector<ParserError> errors;
  std::pair<std::string, Definition*> pair;
  std::vector<Token>::iterator tokenItr;

  // Test whether ParseDict works when the dict does not contain a pair
  // which contains another dict
  GIVEN("Dict does not contain a pair containing a dict") {
    GIVEN("Dict contains only one pair") {
      tokens.push_back(Token(TokenType::IDENTIFIER, "identifier1", 1, 1));
      tokens.push_back(Token(TokenType::PAIRSEPARATOR, ":", 1, 12));
      tokens.push_back(Token(TokenType::IDENTIFIER, "identifier2", 1, 13));
      tokens.push_back(Token(TokenType::DICTDELIMCLOSE, "}", 1, 24));

      tokenItr = tokens.begin();

      THEN("ParseDict should return a pointer to a valid definition") {
        Definition* definitionPtr = parser.parseDict(tokens, tokenItr, errors);

        CHECK(errors.empty());
        REQUIRE(definitionPtr != 0);
        REQUIRE(definitionPtr->pairs.size() == 1);
        REQUIRE(definitionPtr->pairs.at("identifier1") != 0);
        CHECK(definitionPtr->pairs.at("identifier1")->value == "identifier2");
        CHECK(definitionPtr->pairs.at("identifier1")->pairs.empty());
        CHECK(definitionPtr->pairs.at("identifier1")->type == FieldType::Value);
      }
    }

    GIVEN("Dict contains two pairs") {
      tokens.push_back(Token(TokenType::IDENTIFIER, "identifier1", 2, 1));
      tokens.push_back(Token(TokenType::PAIRSEPARATOR, ":", 2, 12));
      tokens.push_back(Token(TokenType::IDENTIFIER, "identifier2", 2, 13));
      tokens.push_back(Token(TokenType::DICTSEPARATOR, ",", 2, 24));
      tokens.push_back(Token(TokenType::IDENTIFIER, "identifier3", 3, 1));
      tokens.push_back(Token(TokenType::PAIRSEPARATOR, ":", 3, 12));
      tokens.push_back(Token(TokenType::IDENTIFIER, "identifier4", 3, 13));
      tokens.push_back(Token(TokenType::DICTDELIMCLOSE, "}", 4, 1));

      tokenItr = tokens.begin();

      THEN("ParseDict should return a pointer to a valid definition") {
        Definition* definitionPtr = parser.parseDict(tokens, tokenItr, errors);

        CHECK(errors.empty());
        REQUIRE(definitionPtr != 0);
        REQUIRE(definitionPtr->pairs.size() == 2);
        REQUIRE(definitionPtr->pairs.at("identifier1") != 0);
        CHECK(definitionPtr->pairs.at("identifier1")->value == "identifier2");
        CHECK(definitionPtr->pairs.at("identifier1")->pairs.empty());
        CHECK(definitionPtr->pairs.at("identifier1")->type == FieldType::Value);
        REQUIRE(definitionPtr->pairs.at("identifier3") != 0);
        CHECK(definitionPtr->pairs.at("identifier3")->value == "identifier4");
        CHECK(definitionPtr->pairs.at("identifier3")->pairs.empty());
        CHECK(definitionPtr->pairs.at("identifier3")->type == FieldType::Value);
      }
    }
  }
}
