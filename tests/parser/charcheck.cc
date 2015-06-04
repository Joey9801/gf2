#include <catch.h>

#include "../../src/parser/charcheck.h"

/// Check that only characters defined as "letters" pass the "letter" check
SCENARIO("Check that only \"letters\" pass \"letter\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if ((currentChar >= 'a' && currentChar <= 'z') ||
        (currentChar >= 'A' && currentChar <= 'Z'))
    {
      result = isLetterChar(currentChar) ? 1 : 0;
    }
    else
    {
      result = isLetterChar(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "digits" pass the "digit" check
SCENARIO("Check that only \"digits\" pass \"digit\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if ((currentChar >= '0' && currentChar <= '9'))
    {
      result = isDigitChar(currentChar) ? 1 : 0;
    }
    else
    {
      result = isDigitChar(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "characters" pass the "character"
/// check
SCENARIO("Check that only \"characters\" pass \"character\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if ((currentChar >= 'a' && currentChar <= 'z') ||
        (currentChar >= 'A' && currentChar <= 'Z') ||
        (currentChar >= '0' && currentChar <= '9') ||
        (currentChar == '_'))
    {
      result = isCharacterChar(currentChar) ? 1 : 0;
    }
    else
    {
      result = isCharacterChar(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "whitespace" pass the "whitespace"
/// check
SCENARIO("Check that only \"whitespace\" characters pass \"whitespace\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if ((currentChar == ' ' ) ||
        (currentChar == '\t') ||
        (currentChar == '\n') ||
        (currentChar == '\v') ||
        (currentChar == '\f') ||
        (currentChar == '\r'))
    {
      result = isWhitespaceChar(currentChar) ? 1 : 0;
    }
    else
    {
      result = isWhitespaceChar(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "punctuation" pass the "punctuation"
/// check
SCENARIO("Check that only \"punctuation\" characters pass \"punctuation\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if ((currentChar >= 33  && currentChar <= 47) ||
        (currentChar >= 58  && currentChar <= 64) ||
        (currentChar >= 91  && currentChar <= 96) ||
        (currentChar >= 123 && currentChar <= 126))
    {
      result = isPunctuationChar(currentChar) ? 1 : 0;
    }
    else
    {
      result = isPunctuationChar(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only valid "string" characters pass the "string" character
/// check
SCENARIO("Check that only valid \"string\" characters pass \"string\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if ((currentChar >= 'a' && currentChar <= 'z') ||
        (currentChar >= 'A' && currentChar <= 'Z') ||
        (currentChar >= '0' && currentChar <= '9') ||
        (currentChar == '_' ) ||
        (currentChar == ' ' ) ||
        (currentChar == '\t') ||
        (currentChar == '\n') ||
        (currentChar == '\v') ||
        (currentChar == '\f') ||
        (currentChar == '\r') ||
        (currentChar >= 33  && currentChar <= 47) ||
        (currentChar >= 58  && currentChar <= 64) ||
        (currentChar >= 91  && currentChar <= 96) ||
        (currentChar >= 123 && currentChar <= 126))
    {
      result = isStringChar(currentChar) ? 1 : 0;
    }
    else
    {
      result = isStringChar(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only valid "comment" characters pass the "comment" character
/// check
SCENARIO("Check that only valid \"comment\" characters pass \"comment\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if ((currentChar >= 'a' && currentChar <= 'z') ||
      (currentChar >= 'A' && currentChar <= 'Z') ||
      (currentChar >= '0' && currentChar <= '9') ||
      (currentChar == '_') ||
      (currentChar == ' ') ||
      (currentChar == '\t') ||
      (currentChar == '\n') ||
      (currentChar == '\v') ||
      (currentChar == '\f') ||
      (currentChar == '\r') ||
      (currentChar >= 33 && currentChar <= 47) ||
      (currentChar >= 58 && currentChar <= 64) ||
      (currentChar >= 91 && currentChar <= 96) ||
      (currentChar >= 123 && currentChar <= 126))
    {
      result = isCommentChar(currentChar) ? 1 : 0;
    }
    else
    {
      result = isCommentChar(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "dict opening delimiters" pass the
/// "dict opening delimiter" check
SCENARIO("Check that only \"dict opening delimiters\" pass \"dict opening delimiter\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if (currentChar == '{')
    {
      result = isOpeningDictDelim(currentChar) ? 1 : 0;
    }
    else
    {
      result = isOpeningDictDelim(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "dict closing delimiters" pass the
/// "dict closing delimiter" check
SCENARIO("Check that only \"dict closing delimiters\" pass \"dict closing delimiter\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if (currentChar == '}')
    {
      result = isClosingDictDelim(currentChar) ? 1 : 0;
    }
    else
    {
      result = isClosingDictDelim(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "dict separators" pass the
/// "dict separator" check
SCENARIO("Check that only \"dict separators\" pass \"dict separator\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if (currentChar == ',')
    {
      result = isDictSeparator(currentChar) ? 1 : 0;
    }
    else
    {
      result = isDictSeparator(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "pair separators" pass the
/// "pair separator" check
SCENARIO("Check that only \"pair separators\" pass \"pair separator\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if (currentChar == ':')
    {
      result = isPairSeparator(currentChar) ? 1 : 0;
    }
    else
    {
      result = isPairSeparator(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "member access operators" pass the
/// "member access operator" check
SCENARIO("Check that only \"member access operators\" pass \"member access operator\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if (currentChar == '.')
    {
      result = isMemberAccessOperator(currentChar) ? 1 : 0;
    }
    else
    {
      result = isMemberAccessOperator(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "vector opening brackets" pass the
/// "vector opening bracket" check
SCENARIO("Check that only \"vector opening brackets\" pass \"vector opening bracket\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if (currentChar == '[')
    {
      result = isOpeningVectorBracket(currentChar) ? 1 : 0;
    }
    else
    {
      result = isOpeningVectorBracket(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}

/// Check that only characters defined as "vector closing brackets" pass the
/// "vector closing bracket" check
SCENARIO("Check that only \"vector closing brackets\" pass \"vector closing bracket\" character check") {
  char currentChar = 0;
  do {
    char result = 0;
    if (currentChar == ']')
    {
      result = isClosingVectorBracket(currentChar) ? 1 : 0;
    }
    else
    {
      result = isClosingVectorBracket(currentChar) ? 0 : 1;
    }
    // The slightly odd way of testing here ensures that the value of the
    // character that has failed will be displayed if a test fails
    result *= currentChar;
    CHECK(int(result) == int(currentChar));
    currentChar++;
  } while (currentChar != 0);
}
