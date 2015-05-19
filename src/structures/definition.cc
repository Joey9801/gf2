#include <iostream>

#include "definition.h"

Definition::Definition() :
  type(Base)
{}

Definition::Definition(FieldType t) :
  type(t)
{}

Definition::Definition(std::string s) :
  type(Value),
  value(s)
{}

Definition::~Definition(void) {
  for(std::map<std::string, Definition*>::iterator it = pairs.begin();
      it != pairs.end();
      it++)
    delete it->second;
}

void Definition::print(void) {
  print(0);
  std::cout << std::endl;
}

void Definition::print(unsigned int depth) {
  if(type == FieldType::Value) {
    std::cout << value;
    return;
  }

  std::cout << "{" << std::endl;
  depth++;

  for(std::map<std::string, Definition*>::iterator it = pairs.begin();
      it != pairs.end();
      it++) {
    std::cout << std::string(2*depth, ' ') << it->first << " : ";
    it->second->print(depth);
    
    std::map<std::string, Definition*>::iterator last = --pairs.end();
    if( it != last )
      std::cout << ",";
    std::cout << std::endl;
  }
  
  depth--;
  std::cout << std::string(2*depth, ' ') << "}";
  return;
}
