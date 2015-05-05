#include <iostream>

#include "simulator/network.h"

int main(void) {
  Network* net = new Network();

  //Create a xor gate out of nand gates as a demo
  //http://upload.wikimedia.org/wikipedia/commons/f/fa/XOR_from_NAND.svg
  //First create two inputs and an output
  unsigned int in1 = net->addInput();
  unsigned int in2 = net->addInput();
  unsigned int out1 = net->addOutput();

  //Then create 4 Nand gates in the network
  unsigned int g1 = net->addComponent("Nand");
  unsigned int g2 = net->addComponent("Nand");
  unsigned int g3 = net->addComponent("Nand");
  unsigned int g4 = net->addComponent("Nand");

  //Internally connect the gates to the network IO
  net->connectInput(in1, g1, 0);
  net->connectInput(in2, g1, 1);
  net->connectInput(in1, g2, 0);
  net->connectInput(in2, g3, 1);
  net->connectOutput(out1, g4, 0);

  //Create the interal connections between the gates
  net->connect(g1, 0, g2, 1);
  net->connect(g1, 0, g3, 0);
  net->connect(g2, 0, g4, 0);
  net->connect(g3, 0, g4, 1);

  //Connect the network to our external nodelist
  net->setInput(in1, 0);
  net->setInput(in2, 1);
  net->setOutput(out1, 2);


  //Our dummy root nodelist
  //The first two elements are wired to the network inputs
  //The last element is wired to the network output
  std::vector<bool> a (3, false);

  //Iterate through all possible inputs
  for(char i=0; i<4; i++){
    a[0] = i & (1<<0);
    a[1] = i & (1<<1);

    //Since network operates synchronously inside, three steps are
    //needed for the new input to fully propagate
    net->step(a, a);
    net->step(a, a);
    net->step(a, a);

    for(int i=0; i<3; i++)
      std::cout << a[i] << ", ";

    std::cout << std::endl;
  }
}
