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
    c->connectOutput(i, _nodesA.size()+i);

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
  _inputDummy->connectOutput(inputId, _nodesA.size()-1);

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

  for(std::map<unsigned int, unsigned int>::iterator it = _monitorPoints.begin();
      it != _monitorPoints.end();
      it++)
    _monitor->setValue( it->first, _nodesA[it->second]);

  return;
}

unsigned int Network::countComponents(void) {
  //-2 to account for the two dummy components
  return _components.size() - 2;
}

void Network::setMonitor(Monitor * m) {
  _monitor = m;
  return;
}

unsigned int Network::addMonitorPoint(std::vector<std::string>& signature) {
  if(not _monitor) {
    // TODO raise an error - monitor not set
  }
  if(signature.size() < 2) {
    // TODO raise an error - not enough information in the signature
    return 0;
  }

  unsigned int componentId = findComponent(signature.back());
  BaseComponent * c = _components[componentId];
  unsigned int pointId = 0;

  if(signature.size() == 2) {
    // This is the network containing the node to monitor
    unsigned int node = c->getOutputNode(signature.front());
    pointId = _monitor->addMonitorPoint();

    _monitorPoints[pointId] = node;
  }
  else {
    //Attempt to cast the component as a Network
    Network * net = dynamic_cast<Network*>(c);
    if(not net) {
      //TODO raise an error, the component in the signature wasn't a network
      return 0;
    }
    signature.pop_back();
    pointId = net->addMonitorPoint(signature);
  }

  return pointId;
}

void Network::removeMonitorPoint(unsigned int pointId) {
  if(_monitorPoints.find(pointId) == _monitorPoints.end()) {
    //TODO raise an error
    return;
  }
  _monitorPoints.erase(pointId);
  // We can't currently remove the logged data in the monitor
  // since it's stored as a simple vector, so removing it
  // would change the indicies of other points
  return;
}

NodeTreeBase * Network::getNodeTree(void) {
  NodeTreeBase * n = new NodeTreeNetwork();

  n->name = _name;

  for(std::vector<BaseComponent*>::iterator it=_components.begin();
      it != _components.end();
      it ++)
    n->children.push_back( (*it)->getNodeTree() );

  for(pin_map::iterator it = _componentNames.begin();
      it != _componentNames.end();
      it ++)
    n->children[(*it).second]->nickname = (*it).first;

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
