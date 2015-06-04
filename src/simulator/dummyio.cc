#include "dummyio.h"

// Dummy IO objects are used as 'virtual' components representing
// the networks inputs and outputs. The networks inputs are represented
// as the the inputDummy's outputs, and the networks outputs by the
// outputDummys inputs.
DummyIO::DummyIO() :
  BaseComponent("dummyio", 0, 0)
{
}

DummyIO::~DummyIO() {}

void DummyIO::step(std::vector<bool>& a, std::vector<bool>& b) {}

void DummyIO::loadInputs(std::vector<bool>& source, std::vector<bool>& sink, std::vector<unsigned int> indicies) {
  for(unsigned int i=0; i<indicies.size(); i++)
    sink[_outputs[i]] = source[indicies[i]];
}

void DummyIO::loadOutputs(std::vector<bool>& source, std::vector<bool>& sink, std::vector<unsigned int> indicies) {
  for(unsigned int i=0; i<indicies.size(); i++)
    sink[indicies[i]] = source[_inputs[i]];
}

unsigned int DummyIO::addInput() {
  _inputs.push_back(0);
  return _inputs.size()-1;
}

unsigned int DummyIO::addInput(std::string name) {
  unsigned int id = addInput();
  renameInput(id, name);
  return id;
}

unsigned int DummyIO::addOutput() {
  _outputs.push_back(0);
  return _outputs.size()-1;
}

unsigned int DummyIO::addOutput(std::string name) {
  unsigned int id = addOutput();
  renameOutput(id, name);
  return id;
}
