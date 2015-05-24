#include "componentconstructors.h"

constructor_map componentConstructor = {
  {"and",     &createComponent<AndGate>},
  {"nand",    &createComponent<NandGate>},
  {"or",      &createComponent<OrGate>},
  {"nor",     &createComponent<NorGate>},
  {"xor",     &createComponent<XorGate>},
  {"dtype",   &createComponent<DType>},
  {"siggen",  &createComponent<SignalGenerator>},
  {"dummyio", &createComponent<DummyIO>}
};
