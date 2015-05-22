#include "network.h"

Network::Network(void) :
  BaseComponent("Network", 0, 0)
{
  LOG_DEBUG;

  //Make room for the const values
  _nodesA.resize(2);
  _nodesB.resize(2);

  //Add the dummy components for IO
  _inputDummy = new DummyIO();
  _outputDummy = new DummyIO();

  _components.push_back( (BaseComponent*)_inputDummy );
  _components.push_back( (BaseComponent*)_outputDummy );

  _componentNames["inputs"] = 0;
  _componentNames["outputs"] = 1;
}

Network::~Network() {}

//returns componentId
unsigned int Network::addComponent(std::string type) {
  if(componentConstructor.find(type) == componentConstructor.end()) {
    //TODO report an error
    throw 1;
    return 0;
  }

  LOG_DEBUG << "type: " << type;

  unsigned int componentId = _components.size();
  BaseComponent * c = componentConstructor[type]();
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
unsigned int Network::addComponent(BaseComponent * c) {

  LOG_DEBUG;

  unsigned int componentId = _components.size();
  for(unsigned int i=0; i<c->numOutputs(); i++)
    c->connectOutput(i, _nodesA.size()+i);

  _components.push_back(c);
  _nodesA.resize(_nodesA.size() + c->numOutputs(), false);
  _nodesB.resize(_nodesA.size());

  return componentId;
}
unsigned int Network::addComponent(BaseComponent * c, std::string name) {
  unsigned int componentId = addComponent(c);
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
    throw 1;
    return;
  }

  LOG_VERBOSE << "oldName: " << oldName << ", newName: " << newName;

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

  LOG_VERBOSE << "componentId: " << componentId << ", newName: " << newName;

  _componentNames[newName] = componentId;
}

unsigned int Network::findComponent(unsigned int componentId) {
  return componentId;
}
unsigned int Network::findComponent(std::string componentName) {
  if(_componentNames.find(componentName) == _componentNames.end()) {
    //TODO raise some sort of error
    throw 1;
    return ~0;
  }
  unsigned int componentId = _componentNames[componentName];
  return componentId;
}

//Returns the inputId
unsigned int Network::addInput(void) {
  LOG_DEBUG;

  unsigned int inputId = _inputs.size();
  _inputs.push_back(0);
  _nodesA.push_back(false);
  _nodesB.push_back(false);

  _inputDummy->addOutput();
  _inputDummy->connectOutput(inputId, _nodesA.size()-1);

  return inputId;
}
unsigned int Network::addInput(std::string name) {
  LOG_DEBUG;

  unsigned int id = addInput();
  renameInput(id, name);
  _inputDummy->renameOutput(id, name);
  return id;
}

unsigned int Network::addOutput(void) {
  LOG_DEBUG;

  unsigned int outputId = _outputs.size();
  _outputs.push_back(0);
  _outputDummy->addInput();

  return outputId;
}
unsigned int Network::addOutput(std::string name) {
  LOG_DEBUG;

  unsigned int id = addOutput();
  renameOutput(id, name);
  _outputDummy->renameInput(id, name);
  return id;
}

void Network::step(std::vector<bool>& a, std::vector<bool>& b) {

  LOG_VERBOSE;

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

  //Start at 2 to  avoid including the DummyIO objects
  for(unsigned int i=2; i<_components.size(); i++)
    n->children.push_back( _components[i]->getNodeTree() );

  for(pin_map::iterator it = _componentNames.begin();
      it != _componentNames.end();
      it ++) {
    //Sim, avoid including the DummyIO objects
    //Ordering of the map is not as added, so if statement needed in every loop
    if( it->second > 1 )
      n->children[(*it).second-2]->nickname = (*it).first;
  }

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

BaseComponent * Network::clone(void) {
  Network * n = new Network();

  n->_components.clear();
  for(std::vector<BaseComponent*>::iterator it = _components.begin();
      it != _components.end();
      it++)
    n->_components.push_back( (*it)->clone() );

  n->_inputDummy = (DummyIO*)n->_components[0];
  n->_outputDummy = (DummyIO*)n->_components[1];

  n->_componentNames = _componentNames;

  n->_nodesA.resize(_nodesA.size(), false);
  n->_nodesB.resize(_nodesB.size(), false);

  n->_inputs = _inputs;
  n->_outputs = _outputs;

  // Give it the monitor object, but don't copy over the monitor points
  n->_monitor = _monitor;

  return (BaseComponent*)n;
}

RootNetwork::RootNetwork(void)
  : Network()
{
}

RootNetwork::~RootNetwork() {}

void RootNetwork::step(void) {
  _nodesA[0] = false;
  _nodesA[1] = true;

  for(std::vector<BaseComponent*>::iterator c = _components.begin();
        c != _components.end();
        c++)
    (*c)->step(_nodesA, _nodesB);

  _nodesA.swap(_nodesB);

  _nodesA[0] = false;
  _nodesA[1] = true;

  for(std::map<unsigned int, unsigned int>::iterator it = _monitorPoints.begin();
      it != _monitorPoints.end();
      it++)
    _monitor->setValue( it->first, _nodesA[it->second]);
}


unsigned int RootNetwork::addInput(void) {
  unsigned int inputId = _inputs.size();
  _inputs.push_back(_nodesA.size());
  _nodesA.push_back(false);
  _nodesB.push_back(false);
  return inputId;
}
unsigned int RootNetwork::addInput(std::string name) {
  unsigned int inputId = addInput();
  renameInput(inputId, name);
  return inputId;
}
unsigned int RootNetwork::addOutput(void) {
  unsigned int outputId = _outputs.size();
  _outputs.push_back(0);
  return outputId;
}
unsigned int RootNetwork::addOutput(std::string name) {
  unsigned int outputId = addOutput();
  renameOutput(outputId, name);
  return outputId;
}

void RootNetwork::setInput(unsigned int inputId, bool value) {
  unsigned int node = _inputs[inputId];
  _nodesA[node] = value;
  return;
}
void RootNetwork::setInput(std::string inputName, bool value) {
  if(_pinInMap.find(inputName) == _pinInMap.end()) {
    // TODO raise an error
    throw 1;
    return;
  }
  unsigned int inputId = _pinInMap[inputName];
  setInput(inputId, value);
  return;
}

bool RootNetwork::getOutput(unsigned int outputId) {
  unsigned int node = _outputs[outputId];
  return _nodesA[node];
}
bool RootNetwork::getOutput(std::string outputName) {
  if(_pinOutMap.find(outputName) == _pinOutMap.end() ) {
    // TODO raise an error
    throw 1;
    return false;
  }
  unsigned int outputId = _pinOutMap[outputName];
  return getOutput(outputId);
}
