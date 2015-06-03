#include "boolgates.h"

BaseBoolgate::BaseBoolgate(std::string name) :
    BaseComponent(name, 0, 1)
{
  _pinOutMap["out"] = 0;
}

void BaseBoolgate::connectInput(std::string inputName, unsigned int node) {
  std::istringstream ss(inputName);
  char leadingchar;
  ss >> leadingchar;

  unsigned int inputId;
  ss >> inputId;

  if(leadingchar != 'i' or ss.fail()) {
    GF2Error e = GF2Error();
    e.name = "Bad input name";
    e.detail = "\"" + inputName + "\" is not an valid input name";
    e.recoverable = false;
    throw e;
  }else if(inputId > 10000) {
    GF2Error e = GF2Error();
    e.name = "Too many inputs";
    e.detail = "Boolean gates with more than 10000 inputs are not supported";
    e.recoverable = false;
    throw e;
  }

  // Names are 1 based in the spec, but our vectors are 0 based
  inputId -= 1;

  connectInput(inputId, node);

  return;
}

void BaseBoolgate::connectInput(unsigned int inputId, unsigned int node) {
  // Allows for an arbitrary number of inputs
  // Unused inputs connected to index#0, the zero constant in networks
  if(inputId >= _inputs.size()){
    std::stringstream ss;
    for(unsigned int i=_inputs.size(); i <= inputId; i++) {
      ss.str(std::string());
      ss << "i" << (i+1);
      _pinInMap[ss.str()] = i;
    }

    _inputs.resize(inputId+1, 0);
  }

  _inputs[inputId] = node;
  return;
}

void BaseBoolgate::step(std::vector<bool>& a, std::vector<bool>& b) {
  bool current = _compare(1, 0) ^ _compare(1, 1);

  for(std::vector<unsigned int>::iterator i = _inputs.begin();
       i != _inputs.end();
       i++)
    current = _compare(current, a[*i]);

  b[_outputs[0]] = current;
  return;
}

XorGate::XorGate() :
  BaseComponent("xor", 2, 1)
{
  _pinInMap["i1"] = 0;
  _pinInMap["i2"] = 1;
  _pinOutMap["out"] = 0;
}

void XorGate::step(std::vector<bool>& a, std::vector<bool>& b) {
  b[_outputs[0]] = a[_inputs[0]] ^ a[_inputs[1]];
  return;
}

bool AndGate::_compare(bool a, bool b) {
  return a && b;
}

bool OrGate::_compare(bool a, bool b) {
  return a || b;
}

// Note that though the following may look like a bug, it is not
bool NandGate::_compare(bool a, bool b) {
  return (a) || (!b);
}

bool NorGate::_compare(bool a, bool b) {
  return (a) && (!b);
}
