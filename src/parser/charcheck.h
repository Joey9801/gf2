/*! \file charcheck.h
          Contains the declarations of functions to check characters
          against the EBNF definitions.
*/

#ifndef CHARCHECK_H_
#define CHARCHECK_H_

bool isLetterChar(const char ch);

bool isDigitChar(const char ch);

bool isCharacterChar(const char ch);

bool isWhitespaceChar(const char ch);

bool isPunctuationChar(const char ch);

bool isStringChar(const char ch);

bool isCommentChar(const char ch);

bool isOpeningDictDelim(const char ch);

bool isClosingDictDelim(const char ch);

bool isDictSeparator(const char ch);

bool isPairSeparator(const char ch);

bool isMemberAccessOperator(const char ch);

bool isOpeningVectorBracket(const char ch);

bool isClosingVectorBracket(const char ch);

bool isValidCharacter(const char ch);

#endif // CHARCHECK_H_
