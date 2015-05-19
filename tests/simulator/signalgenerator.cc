#include <catch.h>

#include "../../src/simulator/basecomponent.h"
#include "../../src/simulator/signalgenerator.h"

SCENARIO("SignalGenerator works as expected", "[signalgenerator]") {
  GIVEN("A newly created SignalGenerator") {
    SignalGenerator s = SignalGenerator();
    std::vector<bool> a(1, false);

    WHEN("Nothing is configured") {
      THEN("Nothing happens") {
        for(unsigned int i=0; i<10; i++) {
          s.step(a, a);
          REQUIRE( a[0] == false );
        }
      }
    }

    WHEN("The data is configured") {
      s.configure("data", "1011");
      THEN("It is cycled with a period of 1") {
        s.step(a, a);
        REQUIRE( a[0] == true );
        s.step(a, a);
        REQUIRE( a[0] == false );
        s.step(a, a);
        REQUIRE( a[0] == true );
        s.step(a, a);
        REQUIRE( a[0] == true );
        s.step(a, a);
        REQUIRE( a[0] == true );
        s.step(a, a);
        REQUIRE( a[0] == false );
      }
    }

    WHEN("The period is also set") {
      s.configure("data", "01");
      s.configure("period", "2");
      THEN("The data is cycled with that period") {
        s.step(a, a);
        REQUIRE( a[0] == false );
        s.step(a, a);
        REQUIRE( a[0] == false );
        s.step(a, a);
        REQUIRE( a[0] == true );
        s.step(a, a);
        REQUIRE( a[0] == true );
        s.step(a, a);
        REQUIRE( a[0] == false );
      }
    }
  }
}

SCENARIO("SignalGenerator is usable upcasted to a BasePointer", "[signalgenerator]") {
  GIVEN("A BaseComponent pointer to a SignalGenerator") {
    BaseComponent * s = new SignalGenerator();

    std::vector<bool> a(1, false);

    WHEN("Everything is configured") {
      s->configure("data", "01");
      s->configure("period", "2");
      THEN("Everything works") {
        s->step(a, a);
        REQUIRE( a[0] == false );
        s->step(a, a);
        REQUIRE( a[0] == false );
        s->step(a, a);
        REQUIRE( a[0] == true );
        s->step(a, a);
        REQUIRE( a[0] == true );
        s->step(a, a);
        REQUIRE( a[0] == false );
      }
    }
  }
}
