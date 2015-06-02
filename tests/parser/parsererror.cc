#include <limits>
#include <catch.h>

#include "../../src/parser/parsererror.h"

SCENARIO("Create some parser errors and check their values") {
  /// Check that using the positionless constructor works
  WHEN("Create the first parser error") {
    ParserError error("The first parser error!");
    REQUIRE(error.getErrorDescription() == "The first parser error!");
    REQUIRE(error.getLineNo() == (unsigned long long int)0);
    REQUIRE(error.getCharNo() == (unsigned long long int)0);
  }

  /// Check that using the constructor with position information works
  WHEN("Create the second parser error") {
    ParserError error("Another error! This time it has a location!", 3, 6);
    REQUIRE(error.getErrorDescription() ==
      "Another error! This time it has a location!");
    REQUIRE(error.getLineNo() == (unsigned long long int)3);
    REQUIRE(error.getCharNo() == (unsigned long long int)6);
  }

  /// Check that the maximum value boundary case works for positions
  WHEN("Create the third parser error") {
    ParserError error(
      "PARSER ERROR: Invalid character at end of largest permittable file!",
      std::numeric_limits<unsigned long long int>::max(),
      std::numeric_limits<unsigned long long int>::max());
    REQUIRE(error.getErrorDescription() ==
      "PARSER ERROR: Invalid character at end of largest permittable file!");
    REQUIRE(error.getLineNo() == std::numeric_limits<unsigned long long int>::max());
    REQUIRE(error.getCharNo() == std::numeric_limits<unsigned long long int>::max());
  }

  /// Check that the minimum value boundary case works for positions
  WHEN("Create the fourth parser error") {
    ParserError error(
      "PARSER ERROR: This test should already have been run using the other constructor!",
      std::numeric_limits<unsigned long long int>::min(),
      std::numeric_limits<unsigned long long int>::min());
    REQUIRE(error.getErrorDescription() ==
      "PARSER ERROR: This test should already have been run using the other constructor!");
    REQUIRE(error.getLineNo() == std::numeric_limits<unsigned long long int>::min());
    REQUIRE(error.getCharNo() == std::numeric_limits<unsigned long long int>::min());
  }
}
