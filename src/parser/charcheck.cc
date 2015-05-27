#include "charcheck.h"

#include <cctype>

/// Check whether the given character is a "letter"
bool isLetterChar(const char ch) {
  return isalpha(ch);
}

/// Check whether the given character is a "digit"
bool isDigitChar(const char ch) {
  return isdigit(ch);
}

/// Check whether the given character is a "character"
bool isCharacterChar(const char ch) {
  return isLetterChar(ch) || isDigitChar(ch) || ch == '_';
}

/// Check whether the given character is a "whitespace" character
bool isWhitespaceChar(const char ch) {
  return isspace(ch);
}

/// Check whether the given character is a "punctuation" character
bool isPunctuationChar(const char ch) {
  return ispunct(ch);
}

/// Check whether the given character is a valid "string" character
bool isStringChar(const char ch) {
  return isCharacterChar(ch) || isWhitespaceChar(ch) || isPunctuationChar(ch);
}

/// Check whether the given character is a valid "comment" character
bool isCommentChar(const char ch) {
  return isCharacterChar(ch) || isWhitespaceChar(ch) || isPunctuationChar(ch);
}

/// Check whether the given character is a valid "dict" opening delimiter
bool isOpeningDictDelim(const char ch) {
  return ch == '{';
}

/// Check whether the given character is a valid "dict" closing delimiter
bool isClosingDictDelim(const char ch) {
  return ch == '}';
}

/// Check whether the given character is a valid "dict" separator
bool isDictSeparator(const char ch) {
  return ch == ',';
}

/// Check whether the given character is a valid "pair" separator
bool isPairSeparator(const char ch) {
  return ch == ':';
}

/// Check whether the given character is any valid character in the language
bool isValidCharacter(const char ch) {
  return  isStringChar(ch)        ||
          isCommentChar(ch)       ||
          isOpeningDictDelim(ch)  ||
          isClosingDictDelim(ch)  ||
          isDictSeparator(ch)     ||
          isPairSeparator(ch);
}
