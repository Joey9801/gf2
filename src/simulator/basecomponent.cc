#include "basecomponent.h"
#include "componentconstructors.h"

extern constructor_map componentConstructor;

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
  LOG_VERBOSE << "Name: " << name << ", numI/O: " << numInputs << "/" << numOutputs;
  _inputs.resize(numInputs);
  _outputs.resize(numOutputs);
}

BaseComponent::~BaseComponent(void)
{
}

void BaseComponent::renameInput(std::string oldName, std::string newName) {
  if(_pinInMap.find(oldName) == _pinInMap.end()) {
    //TODO raise an error
    throw 1;
    return;
  }

  LOG_VERBOSE << "oldName: \"" << oldName << "\", newName: \"" << newName << "\"";

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

  LOG_VERBOSE << "inputId: " << inputId << ", newName: \"" << newName << "\"";

  if(!oldName.empty())
    _pinInMap.erase(oldName);

  _pinInMap[newName] = inputId;

  return;
}

void BaseComponent::renameOutput(std::string oldName, std::string newName) {
  if(_pinOutMap.find(oldName) == _pinOutMap.end()) {
    //TODO raise an error
    throw 1;
    return;
  }

  LOG_VERBOSE << "oldName: \"" << oldName << "\", newName: \"" << newName << "\"";

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

  LOG_VERBOSE << "outputId: " << outputId << ", newName: \"" << newName << "\"";

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
    throw 1;
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
    throw 1;
    return 0;
  }
  unsigned int pinIn = _pinInMap[name];
  return getInputNode(pinIn);
}

unsigned int BaseComponent::numInputs(void) {
  return _inputs.size();
}

void BaseComponent::connectOutput(unsigned int outputId, unsigned int node) {
  _outputs[outputId] = node;
  return;
}

void BaseComponent::connectOutput(std::string name, unsigned int node) {
  if(_pinOutMap.find(name) == _pinOutMap.end()) {
    //TODO raise an error
    throw 1;
    return;
  }
  unsigned int index = _pinOutMap[name];
  connectOutput(index, node);
  return;
}

void BaseComponent::connectInput(unsigned int inputId, unsigned int node) {
  _inputs[inputId] = node;
}

void BaseComponent::connectInput(std::string name, unsigned int node) {
  if(_pinInMap.find(name) == _pinInMap.end()) {
    //TODO raise an error
    throw 1;
    return;
  }

  unsigned int index = _pinInMap[name];
  connectInput(index, node);
}

std::string BaseComponent::getName(void) {
  return _name;
}

NodeTree * BaseComponent::getNodeTree(void) {
  LOG_VERBOSE << "_name = " << _name;
  NodeTree * n = new NodeTree(NodeType::Gate);

  n->name = _name;

  n->inputNodes = _inputs;
  n->outputNodes = _outputs;

  n->inputNames.resize(_pinInMap.size());
  for(pin_map::iterator it=_pinInMap.begin(); it != _pinInMap.end(); it++)
    n->inputNames[(*it).second] = (*it).first;

  n->outputNames.resize(_pinOutMap.size());
  for(pin_map::iterator it=_pinOutMap.begin(); it != _pinOutMap.end(); it++)
    n->outputNames[(*it).second] = (*it).first;

  return n;
}

BaseComponent * BaseComponent::clone(void) {
  BaseComponent * c = componentConstructor[_name]();

  c->_inputs = _inputs;
  c->_outputs = _outputs;
  c->_pinInMap = _pinInMap;
  c->_pinOutMap = _pinOutMap;

  return c;
}
