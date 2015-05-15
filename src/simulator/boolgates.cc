#include "boolgates.h"

BaseBoolgate::BaseBoolgate(std::string name) :
    BaseComponent(name, 0, 1)
{
  std::stringstream stream;
  for(int i=0; i<16; i++) {
    stream.str(std::string());
    stream << "i" << (i+1);
    _pinInMap[stream.str()] = i;
  }
  _pinOutMap["out"] = 0;
}

void BaseBoolgate::setInput(unsigned int inputId, unsigned int node) {
  // Allows for an arbitrary number of inputs
  // Unused inputs connected to index#0, the zero constant in networks
  if(inputId >= _inputs.size()){
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
  BaseComponent("Xor", 2, 1)
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
