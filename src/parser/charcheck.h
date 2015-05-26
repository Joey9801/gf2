/*! \file charcheck.h
          Contains the declarations of functions to check characters
          against the EBNF definitions.
*/

#ifndef CHARCHECK_H_
#define CHARCHECK_H_

bool isLetterChar(char ch);

bool isDigitChar(char ch);

bool isCharacterChar(char ch);

bool isWhitespaceChar(char ch);

bool isPunctuationChar(char ch);

bool isStringChar(char ch);

bool isCommentChar(char ch);

bool isOpeningDictDelim(char ch);

bool isClosingDictDelim(char ch);

bool isDictSeparator(char ch);

bool isPairSeparator(char ch);

bool isValidCharacter(char ch);

#endif // CHARCHECK_H_
