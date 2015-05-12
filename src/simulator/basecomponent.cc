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

unsigned int BaseComponent::getOutputNode(unsigned int pinOut) {
  return _outputs[pinOut];
}

unsigned int BaseComponent::getOutputNode(std::string  name) {
  if(_pinOutMap.find(name) == _pinOutMap.end()){
    //TODO raise an error
    return 0;
  }
  unsigned int pinOut = _pinOutMap[name];
  return getOutputNode(pinOut);
}

unsigned int BaseComponent::numOutputs(void) {
  return _outputs.size();
}

unsigned int BaseComponent::getInputNode(unsigned int pinIn) {
  return _inputs[pinIn];
}

unsigned int BaseComponent::getInputNode(std::string  name) {
  if(_pinInMap.find(name) == _pinInMap.end()){
    //TODO raise an error
    return 0;
  }
  unsigned int pinIn = _pinInMap[name];
  return getInputNode(pinIn);
}

unsigned int BaseComponent::numInputs(void) {
  return _inputs.size();
}

// Each component gets a continuous chunk of nodes for its outputs
// The index argument is the first node in this chunk
void BaseComponent::setOutput(unsigned int outputId, unsigned int node) {
  _outputs[outputId] = node;
  return;
}

void BaseComponent::setInputs(std::vector<unsigned int> nodes) {
  _inputs = nodes;
  return;
}

void BaseComponent::setInput(std::string name, unsigned int node) {
  if(_pinInMap.find(name) == _pinInMap.end()) {
    //TODO raise an error
    return;
  }

  unsigned int index = _pinInMap[name];
  setInput(index, node);
}

void BaseComponent::setInput(unsigned int inputId, unsigned int node) {
  _inputs[inputId] = node;
}

std::string BaseComponent::getName(void) {
  return _name;
}
