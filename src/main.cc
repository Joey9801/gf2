#include <iostream>

#include "simulator/boolgates.h"

int main(void) {
  component_map componentMap;
  componentMap["And"] = &createComponent<AndGate>;
  componentMap["Or"] = &createComponent<OrGate>;
  componentMap["Nand"] = &createComponent<NandGate>;
  componentMap["Nor"] = &createComponent<NorGate>;
  componentMap["Xor"] = &createComponent<XorGate>;

  BaseComponent* gate = componentMap["Xor"]();

  gate->connectInput("I1", 0);
  gate->connectInput("I2", 1);
  gate->connectOutputs(2);

  std::cout << gate->getName() << std::endl;

  std::vector<bool> a (3, false);

  //Iterate through all inputs
  for(char i=0; i<4; i++){
    a[0] = i & (1<<0);
    a[1] = i & (1<<1);
    gate->step(a, a);

    for(int i=0; i<3; i++)
      std::cout << a[i] << ", ";

    std::cout << std::endl;
  }
}
