#include "flipflops.h"

DType::DType() :
  BaseComponent("D-Type", 4, 2)
{
  pinMap["D"] = 0;
  pinMap["S"] = 1;
  pinMap["R"] = 2;
  pinMap["Clock"] = 3;

  _lastIn = false;
  _lastOut = false;
}

DType::~DType() {}

void DType::step(std::vector<bool>& a, std::vector<bool>& b) {
  bool d = a[_inputs[pinMap["D"]]];
  bool s = a[_inputs[pinMap["S"]]];
  bool r = a[_inputs[pinMap["R"]]];
  bool clock = a[_inputs[pinMap["Clock"]]];

  if( s || r ) {
    b[_outputs[0]] = s;
    b[_outputs[1]] = r;
  }
  else if ( (_lastIn == false) && (clock == true) ) {
    //This is a rising edge
    _lastOut = d;
    b[_outputs[0]] = d;
    b[_outputs[1]] = !d;
  }
  else {
    b[_outputs[0]] = _lastOut;
    b[_outputs[1]] = !_lastOut;
  }

  _lastIn = clock;
  return;
}
