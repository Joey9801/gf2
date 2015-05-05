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
  if(pinMap.find(name) == pinMap.end()){
    //TODO raise an error
    return 0;
  }
  unsigned int pinOut = pinMap[name];
  return getOutputNode(pinOut);
}

unsigned int BaseComponent::numOutputs(void) {
  return _outputs.size();
}

unsigned int BaseComponent::getInputNode(unsigned int pinIn) {
  return _outputs[pinIn];
}

unsigned int BaseComponent::getInputNode(std::string  name) {
  if(pinMap.find(name) == pinMap.end()){
    //TODO raise an error
    return 0;
  }
  unsigned int pinIn = pinMap[name];
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
  if(pinMap.find(name) == pinMap.end()) {
    //TODO raise an error
    return;
  }

  unsigned int index = pinMap[name];
  setInput(index, node);
}

void BaseComponent::setInput(unsigned int inputId, unsigned int node) {
  _inputs[inputId] = node;
}

std::string BaseComponent::getName(void) {
  return _name;
}
