#include "flipflops.h"

DType::DType() :
  BaseComponent("D-Type", 4, 2)
{
  _pinInMap["d"] = 0;
  _pinInMap["s"] = 1;
  _pinInMap["r"] = 2;
  _pinInMap["clock"] = 3;

  _pinOutMap["q"] = 0;
  _pinOutMap["qbar"] = 1;

  _lastIn = false;
  _lastOut = false;
}

DType::~DType() {}

void DType::step(std::vector<bool>& a, std::vector<bool>& b) {
  bool d = a[_inputs[_pinInMap["d"]]];
  bool s = a[_inputs[_pinInMap["s"]]];
  bool r = a[_inputs[_pinInMap["r"]]];
  bool clock = a[_inputs[_pinInMap["clock"]]];

  if( s || r ) {
    b[_outputs[_pinOutMap["q"]]] = s;
    b[_outputs[_pinOutMap["qbar"]]] = r;
  }
  else if ( (_lastIn == false) && (clock == true) ) {
    //This is a rising edge
    _lastOut = d;
    b[_outputs[_pinOutMap["q"]]] = d;
    b[_outputs[_pinOutMap["qbar"]]] = !d;
  }
  else {
    b[_outputs[_pinOutMap["q"]]] = _lastOut;
    b[_outputs[_pinOutMap["qbar"]]] = !_lastOut;
  }

  _lastIn = clock;
  return;
}
