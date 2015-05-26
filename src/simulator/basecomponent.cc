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
  if( (name == std::string("")) and (_outputs.size() == 1) ) {
    return _outputs[0];
  }
  else if(_pinOutMap.find(name) == _pinOutMap.end()){
    LOG_ERROR << "\"" << name << "\" not an output of " << _name;
    throw 1;
  }
  unsigned int pinOut = _pinOutMap[name];
  return getOutputNode(pinOut);
}

std::vector<unsigned int> BaseComponent::getOutputVectorNodes(unsigned int pinIn) {
  LOG_ERROR << "HOw did you even get this function to call?!";
  throw 1;
}
std::vector<unsigned int> BaseComponent::getOutputVectorNodes(std::string name) {
  if( not isOutputVector(name) ) {
    LOG_ERROR << "Cannot get vector nodes of \"" << name << "\" as is not an output vector";
    throw 1;
  }

  std::vector<unsigned int> nodes;
  unsigned int size = _outputVectors[name];
  nodes.reserve(size);

  std::stringstream ss;

  for(unsigned int i=0; i<size; i++) {
    ss.str("");
    ss << name << "[" << i << "]";
    nodes.push_back(getOutputNode(ss.str()));
  }

  return nodes;
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
    LOG_ERROR << "\"" << name << "\" not an input of " << _name;
    throw 1;
  }
  unsigned int pinIn = _pinInMap[name];
  return getInputNode(pinIn);
}

std::vector<unsigned int> BaseComponent::getInputVectorNodes(unsigned int pinIn) {
  LOG_ERROR << "HOw did you even get this function to call?!";
  throw 1;
}
std::vector<unsigned int> BaseComponent::getInputVectorNodes(std::string name) {
  if( not isInputVector(name) ) {
    LOG_ERROR << "Cannot get vector nodes of \"" << name << "\" as is not an input vector";
    throw 1;
  }

  std::vector<unsigned int> nodes;
  unsigned int size = _inputVectors[name];
  nodes.reserve(size);

  std::stringstream ss;

  for(unsigned int i=0; i<size; i++) {
    ss.str("");
    ss << name << "[" << i << "]";
    nodes.push_back(getInputNode(ss.str()));
  }

  return nodes;
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
  }

  unsigned int index = _pinInMap[name];
  connectInput(index, node);
}

void BaseComponent::connectVectorInput(unsigned int inputId, std::vector<unsigned int> nodes) {
  LOG_ERROR << "How did you even get this function to call?!";
  throw 1;
}
void BaseComponent::connectVectorInput(std::string name, std::vector<unsigned int> nodes) {
  if( nodes.size() != _inputVectors[name] ) {
    LOG_ERROR << "Trying to connect mismatched vector sizes";
    throw 1;
  }

  LOG_VERBOSE << "name: " << name;

  unsigned int size = nodes.size();
  std::stringstream ss;
  for(unsigned int i=0; i<size; i++) {
    ss.str("");
    ss << name << "[" << i << "]";
    connectInput(ss.str(), nodes[i]);
  }
  return;
}

bool BaseComponent::isInputVector(unsigned int inputId) {
  // ID always references a specific IO
  return false;
}
bool BaseComponent::isOutputVector(unsigned int outputId) {
  // ID always references a specific IO
  return false;
}
bool BaseComponent::isInputVector(std::string inputName) {
  return (_inputVectors.find(inputName) != _inputVectors.end());
}
bool BaseComponent::isOutputVector(std::string outputName) {
  return (_outputVectors.find(outputName) != _outputVectors.end());
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
  c->_inputVectors = _inputVectors;
  c->_outputVectors = _outputVectors;

  return c;
}
