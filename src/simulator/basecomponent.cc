#include "basecomponent.h"

BaseComponent::BaseComponent(std::string name) :
    _name(name)
{
  _inputs.resize(0);
  _outputs.resize(0);
}

BaseComponent::BaseComponent(
    std::string name,
    unsigned int numInputs,
    unsigned int numOutputs)
  :
    _name(name)
{
  _inputs.resize(numInputs);
  _outputs.resize(numOutputs);
}

BaseComponent::~BaseComponent(void)
{
}

// Each component gets a continuous chunk of nodes for its outputs
// The index argument is the first node in this chunk
void BaseComponent::connectOutputs(unsigned int index) {
  for(unsigned int i=0; i<_outputs.size(); i++)
    _outputs[i] = index + i;
  return;
}

void BaseComponent::connectInputs(std::vector<unsigned int> nodes) {
  _inputs = nodes;
  return;
}

void BaseComponent::connectInput(std::string name, unsigned int node) {
  if(pinMap.find(name) == pinMap.end()) {
    //TODO raise an error
    return;
  }

  unsigned int index = pinMap[name];
  connectInput(index, node);
}

void BaseComponent::connectInput(unsigned int index, unsigned int node) {
  _inputs[index] = node;
}

std::string BaseComponent::getName(void) {
  return _name;
}
