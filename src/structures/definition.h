#ifndef DEFINITION_H_
#define DEFINITION_H_

#include <map>
#include <string>

enum FieldType { Base, Dict, Value };

struct Definition {
  Definition();
  Definition(FieldType t);
  Definition(std::string s);

  ~Definition();

  FieldType type;
  std::string filepath;

  std::map<std::string, Definition*> pairs;
  std::string value;

  // Pretty prints the definition to stdout
  // The printed definition will also be a valid definition file
  void print(void);

private:
  void print(unsigned int depth);
};

void printDefinition(Definition * d);
void printDefinition(Definition * d, unsigned int depth);

#endif
