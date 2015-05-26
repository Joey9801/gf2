#include "componentconstructors.h"

constructor_map componentConstructor = {
  {"and",     &createComponent<AndGate>},
  {"nand",    &createComponent<NandGate>},
  {"or",      &createComponent<OrGate>},
  {"nor",     &createComponent<NorGate>},
  {"xor",     &createComponent<XorGate>},
  {"dtype",   &createComponent<DType>},
  {"jk",      &createComponent<JK>},
  {"siggen",  &createComponent<SignalGenerator>},
  {"clock",   &createComponent<Clock>},
  {"dummyio", &createComponent<DummyIO>}
};
