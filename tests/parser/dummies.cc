#include "dummies.h"

namespace Dummy {
  Definition * allBools(void) {
    Definition * def = new Definition(Dict);

    def->pairs["inputs"] = new Definition(Dict);
    def->pairs["outputs"] = new Definition(Dict);
    std::stringstream s1;
    std::stringstream s2;
    for(unsigned int i=0; i<10; i++) {
      s1.str("");
      s1 << "i" << i;
      def->pairs["inputs"]->pairs[s1.str()] = new Definition("false");
    }
    for(unsigned int i=0; i<5; i++) {
      s1.str("");
      s1 << "o" << i;
      s2.str("");
      s2 << "gate" << i << ".out";
      def->pairs["outputs"]->pairs[s1.str()] = new Definition(s2.str());
    }

    //Populate the definition
    def->pairs["components"] = new Definition(Dict);
    Definition * c;

    c = new Definition(Dict);
    c->pairs["type"] = new Definition("and");
    c->pairs["inputs"] = new Definition(Dict);
    c->pairs["inputs"]->pairs["i1"] = new Definition("inputs.i0");
    c->pairs["inputs"]->pairs["i2"] = new Definition("inputs.i1");
    def->pairs["components"]->pairs["gate0"] = c;

    c = new Definition(Dict);
    c->pairs["type"] = new Definition("or");
    c->pairs["inputs"] = new Definition(Dict);
    c->pairs["inputs"]->pairs["i1"] = new Definition("inputs.i2");
    c->pairs["inputs"]->pairs["i2"] = new Definition("inputs.i3");
    def->pairs["components"]->pairs["gate1"] = c;

    c = new Definition(Dict);
    c->pairs["type"] = new Definition("nand");
    c->pairs["inputs"] = new Definition(Dict);
    c->pairs["inputs"]->pairs["i1"] = new Definition("inputs.i4");
    c->pairs["inputs"]->pairs["i2"] = new Definition("inputs.i5");
    def->pairs["components"]->pairs["gate2"] = c;

    c = new Definition(Dict);
    c->pairs["type"] = new Definition("nor");
    c->pairs["inputs"] = new Definition(Dict);
    c->pairs["inputs"]->pairs["i1"] = new Definition("inputs.i6");
    c->pairs["inputs"]->pairs["i2"] = new Definition("inputs.i7");
    def->pairs["components"]->pairs["gate3"] = c;

    c = new Definition(Dict);
    c->pairs["type"] = new Definition("xor");
    c->pairs["inputs"] = new Definition(Dict);
    c->pairs["inputs"]->pairs["i1"] = new Definition("inputs.i8");
    c->pairs["inputs"]->pairs["i2"] = new Definition("inputs.i9");
    def->pairs["components"]->pairs["gate4"] = c;

    return def;
  }
}
