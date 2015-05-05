#include "boolgates.h"

BaseBoolgate::BaseBoolgate(std::string name) :
    BaseComponent(name, 0, 1)
{
  for(int i=0; i<16; i++)
    pinMap["I" + std::to_string(i+1)] = i;
}

void BaseBoolgate::connectInput(unsigned int index, unsigned int node) {
  // Allows for an arbitrary number of inputs
  // Unused inputs connected to index#0, the zero constant in networks
  if(index >= _inputs.size()){
    _inputs.resize(index+1, 0);
  }

  _inputs[index] = node;
  return;
}

void BaseBoolgate::step(std::vector<bool>& a, std::vector<bool>& b) {
  bool current = _compare(1, 0) ^ _compare(1, 1);

  for(unsigned int i : _inputs)
    current = _compare(current, a[i]);

  b[_outputs[0]] = current;
  return;
}

XorGate::XorGate() :
  BaseComponent("Xor", 2, 1)
{
  pinMap["I1"] = 0;
  pinMap["I2"] = 1;
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
