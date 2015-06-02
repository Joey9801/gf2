#include "flipflops.h"

DType::DType() :
  BaseComponent("dtype", 4, 2)
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


JK::JK() :
  BaseComponent("jk", 4, 2)
{
  _pinInMap["j"] = 0;
  _pinInMap["k"] = 1;
  _pinInMap["clock"] = 2;

  _pinOutMap["q"] = 0;
  _pinOutMap["qbar"] = 1;

  _lastIn = false;
  _lastOut = false;
}

JK::~JK() {}

void JK::step(std::vector<bool>& a, std::vector<bool>& b) {
  bool j = a[_inputs[_pinInMap["j"]]];
  bool k = a[_inputs[_pinInMap["k"]]];
  bool clock = a[_inputs[_pinInMap["clock"]]];

  bool out = _lastOut;

  if( clock and (not _lastIn) ) {
    if( j and (not k) )
      out = true;
    if( k and (not j) )
      out = false;
    if( j and k )
      out = not _lastOut;
  }

  b[_outputs[_pinOutMap["q"]]] = out;
  b[_outputs[_pinOutMap["qbar"]]] = not out;

  _lastIn = clock;
  _lastOut = out;
  return;
}
