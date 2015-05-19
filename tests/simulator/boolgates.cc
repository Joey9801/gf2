#include <catch.h>

#include <string>
#include <vector>

#include "../../src/simulator/boolgates.h"

SCENARIO("Inputs to bool gates can be assigned numerical id's", "[boolgates]") {
  GIVEN("A boolgate derived from BaseBoolgate") {
    BaseComponent * gate = new AndGate();

    REQUIRE( gate->numInputs() == 0 );
    REQUIRE( gate->numOutputs() == 1);

    WHEN("Input 0 is assigned") {
      gate->connectInput(0, 0);

      THEN("There is 1 input") {
        REQUIRE( gate->numInputs() == 1);
      }
    }

    WHEN("Input N (N>numInputs()) is assigned") {
      unsigned int N = 3;
      gate->connectInput(N, 0);

      THEN("numInputs is then N+1") {
        REQUIRE( gate->numInputs() == N+1 );
      }
      AND_THEN("The unassigned inputs point to node 0") {
        REQUIRE( gate->getInputNode(0) == 0 );
        REQUIRE( gate->getInputNode(1) == 0 );
        REQUIRE( gate->getInputNode(2) == 0 );
      }
    }
  }
}

SCENARIO("Inputs to bool gates can be assigned by name", "[boolgates]") {
  GIVEN("A boolgate derived from BaseBoolgate") {
    BaseComponent * gate = new AndGate();

    REQUIRE( gate->numInputs() == 0 );
    REQUIRE( gate->numOutputs() == 1);

    WHEN("Input 0 is assigned") {
      gate->connectInput("i1", 0);

      THEN("There is 1 input") {
        REQUIRE( gate->numInputs() == 1);
      }
    }

    WHEN("Input N (N>numInputs()) is assigned") {
      // N=3
      gate->connectInput("i4", 0);

      THEN("numInputs is then N+1") {
        REQUIRE( gate->numInputs() == 4 );
      }
      AND_THEN("The unassigned inputs point to node 0") {
        REQUIRE( gate->getInputNode(0) == 0 );
        REQUIRE( gate->getInputNode(1) == 0 );
        REQUIRE( gate->getInputNode(2) == 0 );
      }
    }
  }
}

SCENARIO("Boolgates produce correct outputs", "[boolgates]") {

  GIVEN("An And gate") {
    BaseComponent * gate = new AndGate();

    WHEN("There are two inputs") {
      std::vector<bool> a(3, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectOutput(0, 2);

      THEN("The truth table is correct") {
        bool in1, in2;
        for(unsigned int i=0; i<4; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          a[0] = in1;
          a[1] = in2;
          gate->step(a, a);
          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == (in1 & in2) );
        }
      }
    }
    
    WHEN("There are three inputs") {
      std::vector<bool> a(4, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectInput(2, 2);
      gate->connectOutput(0, 3);

      THEN("The truth table is correct") {
        bool in1, in2, in3;
        for(unsigned int i=0; i<8; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          in3 = i & (1<<2);
          a[0] = in1;
          a[1] = in2;
          a[2] = in3;
          
          gate->step(a, a);

          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == in3 );
          REQUIRE( a[3] == (in1 & in2 & in3));
        }
      }
    }
  }

  GIVEN("An Or gate") {
    BaseComponent * gate = new OrGate();

    WHEN("There are two inputs") {
      std::vector<bool> a(3, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectOutput(0, 2);

      THEN("The truth table is correct") {
        bool in1, in2;
        for(unsigned int i=0; i<4; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          a[0] = in1;
          a[1] = in2;
          gate->step(a, a);
          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == (in1 | in2) );
        }
      }
    }
    
    WHEN("There are three inputs") {
      std::vector<bool> a(4, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectInput(2, 2);
      gate->connectOutput(0, 3);

      THEN("The truth table is correct") {
        bool in1, in2, in3;
        for(unsigned int i=0; i<8; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          in3 = i & (1<<2);
          a[0] = in1;
          a[1] = in2;
          a[2] = in3;
          
          gate->step(a, a);

          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == in3 );
          REQUIRE( a[3] == (in1 | in2 | in3));
        }
      }
    }
  }

  GIVEN("A Nand gate") {
    BaseComponent * gate = new NandGate();

    WHEN("There are two inputs") {
      std::vector<bool> a(3, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectOutput(0, 2);

      THEN("The truth table is correct") {
        bool in1, in2;
        for(unsigned int i=0; i<4; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          a[0] = in1;
          a[1] = in2;
          gate->step(a, a);
          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == !(in1 & in2) );
        }
      }
    }
    
    WHEN("There are three inputs") {
      std::vector<bool> a(4, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectInput(2, 2);
      gate->connectOutput(0, 3);

      THEN("The truth table is correct") {
        bool in1, in2, in3;
        for(unsigned int i=0; i<8; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          in3 = i & (1<<2);
          a[0] = in1;
          a[1] = in2;
          a[2] = in3;
          
          gate->step(a, a);

          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == in3 );
          REQUIRE( a[3] == !(in1 & in2 & in3));
        }
      }
    }
  }

  GIVEN("A Nor gate") {
    BaseComponent * gate = new NorGate();

    WHEN("There are two inputs") {
      std::vector<bool> a(3, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectOutput(0, 2);

      THEN("The truth table is correct") {
        bool in1, in2;
        for(unsigned int i=0; i<4; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          a[0] = in1;
          a[1] = in2;
          gate->step(a, a);
          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == !(in1 | in2) );
        }
      }
    }
    
    WHEN("There are three inputs") {
      std::vector<bool> a(4, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectInput(2, 2);
      gate->connectOutput(0, 3);

      THEN("The truth table is correct") {
        bool in1, in2, in3;
        for(unsigned int i=0; i<8; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          in3 = i & (1<<2);
          a[0] = in1;
          a[1] = in2;
          a[2] = in3;
          
          gate->step(a, a);

          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == in3 );
          REQUIRE( a[3] == !(in1 | in2 | in3));
        }
      }
    }
  }

  GIVEN("A Xor gate") {
    BaseComponent * gate = new XorGate();

    WHEN("There are two inputs") {
      std::vector<bool> a(3, false);
      gate->connectInput(0, 0);
      gate->connectInput(1, 1);
      gate->connectOutput(0, 2);

      THEN("The truth table is correct") {
        bool in1, in2;
        for(unsigned int i=0; i<4; i++){
          in1 = i & (1<<0);
          in2 = i & (1<<1);
          a[0] = in1;
          a[1] = in2;
          gate->step(a, a);
          REQUIRE( a[0] == in1 );
          REQUIRE( a[1] == in2 );
          REQUIRE( a[2] == (in1 ^ in2) );
        }
      }
    }
  }
}
