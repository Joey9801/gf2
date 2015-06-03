#include <iostream>

#include "parser/parser.h"

using namespace std;

//This is a dummy main, feel free to overwrite it in any merge
int main(void) {

  string filename("tests/parser/test_files/test1.def");

  Parser parser;
  Lexor lexor;

  vector<ParserError> errors;
  vector<Token> tokens;

  lexor.lex(filename, errors, tokens);

  printf("LEXING OUTPUT:\n\n");

  for (vector<Token>::iterator itr = tokens.begin(); itr != tokens.end(); ++itr) {
    printf( "Line: %u, Char: %u, Token: %s\n",
            (int)itr->getStartLineNo(),
            (int)itr->getStartCharNo(),
            itr->getValue().c_str());
  }
  for (vector<ParserError>::iterator itr = errors.begin(); itr != errors.end(); ++itr) {
    printf("Line: %u, Char: %u, Error: %s\n",
      (int)itr->getLineNo(),
      (int)itr->getCharNo(),
      itr->getErrorDescription().c_str());
  }

  errors.clear();

  printf("\n\n\n");

  Definition* def = parser.parse(filename, errors);

  printf("PARSING OUTPUT:\n\n");

  for (vector<ParserError>::iterator itr = errors.begin(); itr != errors.end(); ++itr) {
    printf("Line: %u, Char: %u, Error: %s\n",
      (int)itr->getLineNo(),
      (int)itr->getCharNo(),
      itr->getErrorDescription().c_str());
  }
  def->print();

  errors.clear();

  printf("\n\n\n");

  return 0;
}
