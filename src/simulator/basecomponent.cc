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

void BaseComponent::renameInput(std::string oldName, std::string newName) {
  if(_pinInMap.find(oldName) == _pinInMap.end()) {
    //TODO raise an error
    return;
  }

  unsigned int index = _pinInMap[oldName];
  _pinInMap.erase(oldName);
  _pinInMap[newName] = index;
  
  return;
}

void BaseComponent::renameInput(unsigned int inputId, std::string newName) {
  std::string oldName;
  for(pin_map::iterator it = _pinInMap.begin();
      it != _pinInMap.end();
      it++) {
    if( it->second == inputId )
      oldName = it->first;
  }

  if(!oldName.empty())
    _pinInMap.erase(oldName);

  _pinInMap[newName] = inputId;

  return;
}

void BaseComponent::renameOutput(std::string oldName, std::string newName) {
  if(_pinOutMap.find(oldName) == _pinOutMap.end()) {
    //TODO raise an error
    return;
  }

  unsigned int index = _pinOutMap[oldName];
  _pinOutMap.erase(oldName);
  _pinOutMap[newName] = index;
  
  return;
}

void BaseComponent::renameOutput(unsigned int outputId, std::string newName) {
  std::string oldName;
  for(pin_map::iterator it = _pinOutMap.begin();
      it != _pinOutMap.end();
      it++) {
    if( it->second == outputId )
      oldName = it->first;
  }

  _pinOutMap.erase(oldName);
  _pinOutMap[newName] = outputId;

  return;
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

void BaseComponent::setOutput(unsigned int outputId, unsigned int node) {
  _outputs[outputId] = node;
  return;
}

void BaseComponent::setOutput(std::string name, unsigned int node) {
  if(_pinOutMap.find(name) == _pinOutMap.end()) {
    //TODO raise an error
    return;
  }
  unsigned int index = _pinOutMap[name];
  setOutput(index, node);
  return;
}

void BaseComponent::setInput(unsigned int inputId, unsigned int node) {
  _inputs[inputId] = node;
}

void BaseComponent::setInput(std::string name, unsigned int node) {
  if(_pinInMap.find(name) == _pinInMap.end()) {
    //TODO raise an error
    return;
  }

  unsigned int index = _pinInMap[name];
  setInput(index, node);
}

std::string BaseComponent::getName(void) {
  return _name;
}

NodeTreeBase * BaseComponent::getNodeTree(void) {
  NodeTreeBase * n = new NodeTreeGate();

  n->name = _name;

  n->inputNodes = _inputs;
  n->outputNodes = _outputs;

  n->inputNames.resize(_pinInMap.size());
  for(pin_map::iterator it=_pinInMap.begin(); it != _pinInMap.end(); it++)
    n->inputNames[(*it).second] = (*it).first;

  n->outputNames.resize(_pinInMap.size());
  for(pin_map::iterator it=_pinOutMap.begin(); it != _pinOutMap.end(); it++)
    n->outputNames[(*it).second] = (*it).first;

  return n;
}
