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
