#include <catch.h>
#include <vector>
#include <sstream>

#include "../../src/parser/builder.h"
#include "../../src/structures/definition.h"
#include "../../src/simulator/network.h"
#include "dummies.h"

SCENARIO("A flat definition with only IO connections can be built") {
  GIVEN("A flat definition object") {
    Definition * def = Dummy::allBools();

    WHEN("The network is built") {
      Network * net = Builder::buildNetwork(def);

      REQUIRE( net->countComponents() == 5 );
      REQUIRE( net->numInputs() == 10 );
      REQUIRE( net->numOutputs() == 5);

      THEN("The network behaves as expected") {
          std::vector<bool> a(15, false);

          for(unsigned int i=0; i<10; i++) {
            net->connectInput(i, i);
          }

          for(unsigned int i=0; i<5; i++) {
            net->connectOutput(i, 10+i);
          }

          for(unsigned int i=0; i<4; i++) {
            bool in1 = i & (1<<0);
            bool in2 = i & (1<<1);

            //Apply the inputs
            for(unsigned int j=0; j<10; j+=2) {
              a[j] = in1;
              a[j+1] = in2;
            }

            net->step(a, a);

            CAPTURE( in1 );
            CAPTURE( in2 );

            //Check that the inputs haven't changed
            for(unsigned int j=0; j<10; j+=2) {
              REQUIRE( a[j] == in1 );
              REQUIRE( a[j+1] == in2 );
            }

            //Check whether the outputs were set properly
            REQUIRE( a[10] == (in1 & in2) );
            REQUIRE( a[11] == (in1 || in2) );
            REQUIRE( a[12] == not (in1 & in2) );
            REQUIRE( a[13] == not (in1 || in2) );
            REQUIRE( a[14] == (in1 ^ in2) );
          }
      }
    }
  }
}
