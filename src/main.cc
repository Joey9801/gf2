#include <iostream>

#include "structures/definition.h"

//This is a dummy main, feel free to overwrite it in any merge
int main(void) {

  //Manually create a definition as a demo
  Definition d = Definition(Dict);

  d.pairs["inputs"] = new Definition(Dict);
  d.pairs["inputs"]->pairs["one"] = new Definition("true");
  d.pairs["inputs"]->pairs["two"] = new Definition("false");

  d.pairs["components"] = new Definition(Dict);
  Definition * c1 = new Definition(Dict);
  c1->pairs["type"] = new Definition("nand");
  c1->pairs["inputs"] = new Definition(Dict);
  c1->pairs["inputs"]->pairs["i1"] = new Definition("inputs.one");
  c1->pairs["inputs"]->pairs["i2"] = new Definition("inputs.one");

  d.pairs["components"]->pairs["gate_1"] = c1;

  d.print();

  return 0;
}
