#include <catch.h>

#include "../src/simulator/network.h"

TEST_CASE("Components can be added to the network", "[network]") {
  GIVEN("A network") {
    Network * net = new Network();
    REQUIRE( net->countComponents() == 0 );

    WHEN("A component is added") {
      net->addComponent("And");
      THEN("The component count is 1") {
        REQUIRE( net->countComponents() == 1 );
      }
    }

    WHEN("N components are added") {
      net->addComponent("And");
      net->addComponent("And");
      net->addComponent("And");
      net->addComponent("And");

      THEN("The component count is N") {
        REQUIRE( net->countComponents() == 4 );
      }
    }
  }
}

TEST_CASE("gates can be connected to the network IO", "[network]") {
  GIVEN("A network containing a single boolgate") {
    Network* net = new Network();
    unsigned int in1 = net->addInput();
    unsigned int in2 = net->addInput();
    unsigned int out1 = net->addOutput();

    unsigned int gate = net->addComponent("Xor");

    WHEN("The IO is connected by numeric id") {
      //Connect the gate to the network IO
      net->connect(0, in1, gate, 0);
      net->connect(0, in2, gate, 1);
      net->connect(gate, 0, 1, out1);

      //Connect the network IO the root nodelist
      net->setInput(in1, 0);
      net->setInput(in2, 1);
      net->setOutput(out1, 2);

      //root nodelist
      std::vector<bool> nodes(3, false);

      THEN("The network responds as expected") {
        bool A, B;
        for(char i=0; i<4; i++){
          A = i & (1<<0);
          B = i & (1<<1);
          nodes[0] = A;
          nodes[1] = B;

          net->step(nodes, nodes);
          
          REQUIRE( nodes[0] == A );
          REQUIRE( nodes[1] == B );
          REQUIRE( nodes[2] == (A ^ B) );
        }
      }
    }
  }
}

TEST_CASE("gates can be connected internally", "[network]") {
  GIVEN("4 Nand gates connected as a Xor gate") {
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
    net->connect(0, in1, g1, 0);
    net->connect(0, in2, g1, 1);
    net->connect(0, in1, g2, 0);
    net->connect(0, in2, g3, 1);
    net->connect(g4, 0, 1, out1);

    //Create the interal connections between the gates
    net->connect(g1, 0, g2, 1);
    net->connect(g1, 0, g3, 0);
    net->connect(g2, 0, g4, 0);
    net->connect(g3, 0, g4, 1);

    //Connect the network to the root nodelist
    net->setInput(in1, 0);
    net->setInput(in2, 1);
    net->setOutput(out1, 2);

    std::vector<bool> nodes(3, false);

    WHEN("The network inputs are changed") {
      THEN("The right outputs are observed") {
        bool A, B;
        for(char i=0; i<4; i++){
          A = i & (1<<0);
          B = i & (1<<1);
          nodes[0] = A;
          nodes[1] = B;

          //Need three steps to ensure inputs have fully propagated
          net->step(nodes, nodes);
          net->step(nodes, nodes);
          net->step(nodes, nodes);
          
          REQUIRE( nodes[0] == A );
          REQUIRE( nodes[1] == B );
          REQUIRE( nodes[2] == (A ^ B) );
        }
      }
    }
  }
}
