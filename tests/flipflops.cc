#include <vector>

#include <catch.h>

#include "../src/simulator/flipflops.h"

TEST_CASE("D-type flip flop works as expected") {
  GIVEN("A D-Type flip flop") {
    BaseComponent * c = new DType();

    std::vector<bool> nodes(6, false);

    c->setInput("d", 0);
    c->setInput("clock", 1);
    c->setInput("s", 2);
    c->setInput("r", 3);

    c->setOutput(0, 4);
    c->setOutput(1, 5);

    WHEN("The clock is low") {
      nodes[1] = false;
      c->step(nodes, nodes);
      bool lastQ = nodes[4];
      bool lastQBar = nodes[5];

      THEN("D does nothing") {
        nodes[0] = false;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );

        nodes[0] = true;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );

        nodes[0] = false;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );
      }
    }
    
    WHEN("The clock is high") {
      nodes[1] = true;
      c->step(nodes, nodes);
      bool lastQ = nodes[4];
      bool lastQBar = nodes[5];

      THEN("D does nothing") {
        nodes[0] = false;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );

        nodes[0] = true;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );

        nodes[0] = false;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );
      }
    }

    WHEN("The clock is rising") {
      THEN("The output captures D") {
        nodes[0] = false;
        nodes[1] = false;
        c->step(nodes, nodes);
        nodes[1] = true;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == false );
        REQUIRE( nodes[5] == true );

        nodes[0] = true;
        nodes[1] = false;
        c->step(nodes, nodes);
        nodes[1] = true;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == true );
        REQUIRE( nodes[5] == false );
      }
    }

    WHEN("The clock is falling") {
      nodes[1] = true;
      c->step(nodes, nodes);
      bool lastQ = nodes[4];
      bool lastQBar = nodes[5];

      nodes[1] = false;

      THEN("The output remains the same") {
        nodes[0] = false;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );
      }
      THEN("The output remains the same") {
        nodes[0] = true;
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == lastQ );
        REQUIRE( nodes[5] == lastQBar );
      }
    }

    WHEN("S is high") {
      nodes[2] = true;
      THEN("The output is set") {
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == true );
        REQUIRE( nodes[5] == false );
      }
    }
    WHEN("R is high") {
      nodes[3] = true;
      THEN("The output is unset") {
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == false );
        REQUIRE( nodes[5] == true );
      }
    }
    WHEN("Both R and S are high") {
      nodes[2] = true;
      nodes[3] = true;
      THEN("Both outputs are true") {
        c->step(nodes, nodes);
        REQUIRE( nodes[4] == true );
        REQUIRE( nodes[5] == true );
      }
    }
  }
}
