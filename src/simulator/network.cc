#include "network.h"

// Dummy IO objects are used as 'virtual' components representing
// the networks inputs and outputs. The networks inputs are represented
// as the the inputDummy's outputs, and the networks outputs by the
// outputDummys inputs.
DummyIO::DummyIO() :
  BaseComponent("Dummy IO", 0, 0)
{
  std::stringstream stream;
  for(int i=0; i<16; i++) {
    stream.str(std::string());
    stream << "i" << (i+1);
    _pinInMap[stream.str()] = i;
    _pinOutMap[stream.str()] = i;
  }
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
  unsigned int id = addInput(0);
  renameInput(id, name);
  return id;
}

unsigned int DummyIO::addOutput() {
  _outputs.push_back(0);
  return _outputs.size()-1;
}

unsigned int DummyIO::addOutput(std::string name) {
  unsigned int id = addOutput(0);
  renameOutput(id, name);
  return id;
}


Network::Network(void) :
  BaseComponent("Network", 0, 0)
{
  _componentConstructor["And"] = &createComponent<AndGate>;
  _componentConstructor["Nand"] = &createComponent<NandGate>;
  _componentConstructor["Or"] = &createComponent<OrGate>;
  _componentConstructor["Nor"] = &createComponent<NorGate>;
  _componentConstructor["Xor"] = &createComponent<XorGate>;
  _componentConstructor["DType"] = &createComponent<DType>;
  _componentConstructor["SigGen"] = &createComponent<SignalGenerator>;

  //Make room for the const values
  _nodesA.resize(2);
  _nodesB.resize(2);

  //Add the dummy components for IO
  _inputDummy = new DummyIO();
  _outputDummy = new DummyIO();

  _components.push_back( (BaseComponent*)_inputDummy );
  _components.push_back( (BaseComponent*)_outputDummy );

  _componentNames["input"] = 0;
  _componentNames["output"] = 1;
}

Network::~Network() {}

//returns componentId
unsigned int Network::addComponent(std::string type) {
  if(_componentConstructor.find(type) == _componentConstructor.end()) {
    //TODO report an error
    return 0;
  }

  unsigned int componentId = _components.size();
  BaseComponent * c = _componentConstructor[type]();
  for(unsigned int i=0; i<c->numOutputs(); i++)
    c->setOutput(i, _nodesA.size()+i);

  _components.push_back(c);
  _nodesA.resize(_nodesA.size() + c->numOutputs(), false);
  _nodesB.resize(_nodesA.size());

  return componentId;
}
unsigned int Network::addComponent(std::string type, std::string name) {
  unsigned int componentId = addComponent(type);
  renameComponent(componentId, name);
  return componentId;
}

void Network::configureComponent(std::string name, std::string key, std::string value) {
  unsigned int componentId = findComponent(name);
  BaseComponent * c = _components[componentId];
  c->configure(key, value);
  return;
}

void Network::configureComponent(unsigned int componentId, std::string key, std::string value) {
  BaseComponent * c = _components[componentId];
  c->configure(key, value);
  return;
}

void Network::renameComponent(std::string oldName, std::string newName) {
  if(_componentNames.find(oldName) == _componentNames.end()) {
    //TODO raise some sort of error
    return;
  }

  unsigned int componentId = _componentNames[oldName];
  _componentNames.erase(oldName);
  _componentNames[newName] = componentId;

  return;
}
void Network::renameComponent(unsigned int componentId, std::string newName) {
  //Remove the old name (if there was one)
  std::string oldName;
  for(std::map<std::string, unsigned int>::iterator it = _componentNames.begin();
      it != _componentNames.end();
      it++) {
    if( it->second == componentId )
      oldName = it->first;
  }
  if(!oldName.empty())
    _componentNames.erase(oldName);

  _componentNames[newName] = componentId;
}

unsigned int Network::findComponent(unsigned int componentId) {
  return componentId;
}
unsigned int Network::findComponent(std::string componentName) {
  if(_componentNames.find(componentName) == _componentNames.end()) {
    //TODO raise some sort of error
    return ~0;
  }
  unsigned int componentId = _componentNames[componentName];
  return componentId;
}

//Returns the inputId
unsigned int Network::addInput(void) {
  unsigned int inputId = _inputs.size();
  _inputs.push_back(0);
  _nodesA.push_back(false);
  _nodesB.push_back(false);

  _inputDummy->addOutput();
  _inputDummy->setOutput(inputId, _nodesA.size()-1);

  return inputId;
}
unsigned int Network::addInput(std::string name) {
  unsigned int id = addInput();
  renameInput(id, name);
  _inputDummy->renameOutput(id, name);
  return id;
}

unsigned int Network::addOutput(void) {
  unsigned int outputId = _outputs.size();
  _outputs.push_back(0);
  _outputDummy->addInput();

  return outputId;
}
unsigned int Network::addOutput(std::string name) {
  unsigned int id = addOutput();
  renameOutput(id, name);
  _outputDummy->renameInput(id, name);
  return id;
}

void Network::step(std::vector<bool>& a, std::vector<bool>& b) {

  _inputDummy->loadInputs(a, _nodesA, _inputs);

  _nodesA[0] = false;
  _nodesA[1] = true;

  for(std::vector<BaseComponent*>::iterator c = _components.begin();
        c != _components.end();
        c++)
    (*c)->step(_nodesA, _nodesB);

  _nodesA.swap(_nodesB);

  _nodesA[0] = false;
  _nodesA[1] = true;

  _outputDummy->loadOutputs(_nodesA, b, _outputs);

  return;
}

unsigned int Network::countComponents(void) {
  //-2 to account for the two dummy components
  return _components.size() - 2;
}
