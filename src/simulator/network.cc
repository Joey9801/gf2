#include "network.h"

Network::Network(void) :
  BaseComponent("Network", 0, 0),
  numConnections(0),
  _time(0),
  _rate(1),
  _async(false)
{
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
  }

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
    if(componentName == "const")
      return compl 0;

    LOG_ERROR << "\"" << componentName << "\" is not a componentName in the current network";
    throw 1;
  }
  unsigned int componentId = _componentNames[componentName];
  return componentId;
}

unsigned int Network::constNode(unsigned int constVal) {
  return (constVal==1) ? 1 : 0;
}
unsigned int Network::constNode(std::string constVal) {
  return (constVal=="true") ? 1 : 0;
}

//Returns the inputId
unsigned int Network::addInput(void) {
  LOG_VERBOSE;

  unsigned int inputId = _inputs.size();
  _inputs.push_back(0);
  _nodesA.push_back(false);
  _nodesB.push_back(false);

  _inputDummy->addOutput();
  _inputDummy->connectOutput(inputId, _nodesA.size()-1);

  return inputId;
}
unsigned int Network::addInput(std::string name) {
  LOG_VERBOSE;

  unsigned int id = addInput();
  renameInput(id, name);
  _inputDummy->renameOutput(id, name);
  return id;
}

unsigned int Network::addOutput(void) {
  LOG_VERBOSE;

  unsigned int outputId = _outputs.size();
  _outputs.push_back(0);
  _outputDummy->addInput();

  return outputId;
}
unsigned int Network::addOutput(std::string name) {
  LOG_VERBOSE;

  unsigned int id = addOutput();
  renameOutput(id, name);
  _outputDummy->renameInput(id, name);
  return id;
}

unsigned int Network::addVectorInput(std::string signature) {
  // Creates a vector input from a string in the form "name[num]"
  // Returns the inputId of the first one
  if((signature.find_first_of('[') == std::string::npos)
  or (signature.find_first_of(']') == std::string::npos)) {
    LOG_ERROR << "Tried to create a vector input with invalid signature: " << signature;
    throw 1;
  }

  std::string name = signature.substr(0, signature.find_first_of('['));

  unsigned int size;
  std::stringstream ss;
  ss.str(signature.substr(signature.find_first_of('[')+1, signature.find_first_of(']')));
  ss >> size;

  _inputVectors[name] = size;
  _inputDummy->_inputVectors[name] = size;

  for(unsigned int i=0; i<size; i++) {
    ss.str("");
    ss << name << "[" << i << "]";
    addInput(ss.str());
  }
  ss.str("");
  ss << name << "[0]";
  return _pinInMap[ss.str()];
}
unsigned int Network::addVectorOutput(std::string signature) {
  // Creates a vector output from a string in the form "name[num]"
  // Returns the outputId of the first one
  if((signature.find_first_of('[') == std::string::npos)
  or (signature.find_first_of(']') == std::string::npos)) {
    LOG_ERROR << "Tried to create a vector output with invalid signature: " << signature;
    throw 1;
  }

  std::string name = signature.substr(0, signature.find_first_of('['));

  unsigned int size;
  std::stringstream ss;
  ss.str(signature.substr(signature.find_first_of('[')+1, signature.find_first_of(']')));
  ss >> size;

  _outputVectors[name] = size;
  _outputDummy->_outputVectors[name] = size;

  for(unsigned int i=0; i<size; i++) {
    ss.str("");
    ss << name << "[" << i << "]";
    addOutput(ss.str());
  }
  ss.str("");
  ss << name << "[0]";
  return _pinOutMap[ss.str()];
}

void Network::step(std::vector<bool>& a, std::vector<bool>& b) {

  LOG_VERBOSE;

  _inputDummy->loadInputs(a, _nodesA, _inputs);

  _nodesA[0] = false;
  _nodesA[1] = true;

  for(unsigned int i=0; i<_rate; i++) {
    if(_async) {
      for(std::vector<BaseComponent*>::iterator c = _components.begin();
          c != _components.end();
          c++)
        (*c)->step(_nodesA, _nodesA);
    }
    else {
      for(std::vector<BaseComponent*>::iterator c = _components.begin();
          c != _components.end();
          c++)
        (*c)->step(_nodesA, _nodesB);

      _nodesA.swap(_nodesB);
    }
  }

  _nodesA[0] = false;
  _nodesA[1] = true;

  _outputDummy->loadOutputs(_nodesA, b, _outputs);

  for(std::map<unsigned int, unsigned int>::iterator it = _monitorPoints.begin();
      it != _monitorPoints.end();
      it++)
    _monitor->setValue( it->first, _time, _nodesA[it->second]);

  _time++;

  return;
}

unsigned int Network::countComponents(void) {
  //-2 to account for the two dummy components
  return _components.size() - 2;
}

void Network::setMonitor(Monitor * m) {
  _monitor = m;
  for(std::vector<BaseComponent*>::iterator it = _components.begin();
      it != _components.end();
      it++) {
    Network * net = dynamic_cast<Network*>(*it);
    if(net)
      net->setMonitor(m);
  }
  return;
}
Monitor * Network::getMonitor(void) {
  return _monitor;
}

unsigned int Network::addMonitorPoint(std::vector<std::string>& signature) {
  return addMonitorPoint(signature, 0);
}

unsigned int Network::addMonitorPoint(std::vector<std::string>& signature, unsigned int depth) {
  if(not _monitor) {
    // TODO raise an error - monitor not set
    LOG_ERROR << "Cannot set monitor point without Monitor object";
    throw 1;
  }

  unsigned int remaining = signature.size() - depth;
  if(remaining < 2) {
    // TODO raise an error - not enough information in the signature
    LOG_ERROR << "Not enough information in signature to add monitor point";
    throw 1;
  }

  unsigned int componentId = findComponent(signature[remaining-1]);
  BaseComponent * c = _components[componentId];
  unsigned int pointId = 0;

  if(remaining == 2) {
    // This is the network containing the node to monitor
    unsigned int node = c->getOutputNode(signature.front());
    pointId = _monitor->addPoint(signature);

    _monitorPoints[pointId] = node;
  }
  else {
    //Attempt to cast the component as a Network
    Network * net = dynamic_cast<Network*>(c);
    if(not net) {
      //TODO raise an error, the component in the signature wasn't a network
      LOG_ERROR << "The monitor point signature was not found in the network";
      throw 1;
    }
    pointId = net->addMonitorPoint(signature, depth+1);
  }

  return pointId;
}

void Network::removeMonitorPoint(std::vector<std::string>& signature) {
  removeMonitorPoint(signature, 0);
  return;
}

void Network::removeMonitorPoint(std::vector<std::string>& signature, unsigned int depth) {
  if(not _monitor) {
    // TODO raise an error - monitor not set
    LOG_ERROR << "Cannot set monitor point without Monitor object";
    throw 1;
  }

  unsigned int remaining = signature.size() - depth;
  if(remaining < 2) {
    // TODO raise an error - not enough information in the signature
    LOG_ERROR << "Not enough information in signature to remove monitor point";
    throw 1;
  }

  unsigned int componentId = findComponent(signature[remaining-1]);
  BaseComponent * c = _components[componentId];

  if(remaining == 2) {
    // This is the network containing the node to monitor
    unsigned int pointId = _monitor->findPoint(signature);
    _monitor->removePoint(signature);
    _monitorPoints.erase(pointId);
  }
  else {
    //Attempt to cast the component as a Network
    Network * net = dynamic_cast<Network*>(c);
    if(not net) {
      //TODO raise an error, the component in the signature wasn't a network
      LOG_ERROR << "The monitor point signature was not found in the network";
      throw 1;
    }
    net->removeMonitorPoint(signature, depth+1);
  }

  return;
}

void Network::configure(std::string key, std::string value) {
  if(key=="rate") {
    std::istringstream( value ) >> _rate;
  }
  else if(key=="simulation")
    _async = (value=="async");

  return;
}

NodeTree * Network::getNodeTree(void) {
  NodeTree * n = new NodeTree(NodeType::Network);

  n->name = _name;

  for(unsigned int i=0; i<_components.size(); i++) {
    n->children.push_back( _components[i]->getNodeTree() );
    n->children.back()->parent = n;
  }

  for(pin_map::iterator it = _componentNames.begin();
      it != _componentNames.end();
      it ++) {
    n->children[(*it).second]->nickname = (*it).first;
  }

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

Definition * Network::getDefinition(void) {
  return _definition;
}

void Network::setDefinition(Definition * def) {
  _definition = def;
  return;
}

BaseComponent * Network::clone(void) {
  Network * n = new Network();

  for(unsigned int i=2; i<_components.size(); i++)
    n->_components.push_back( _components[i]->clone() );

  n->_componentNames = _componentNames;

  n->_nodesA.resize(_nodesA.size(), false);
  n->_nodesB.resize(_nodesB.size(), false);

  n->_inputs = _inputs;
  n->_outputs = _outputs;
  n->_inputVectors = _inputVectors;
  n->_outputVectors = _outputVectors;

  n->_pinInMap = _pinInMap;
  n->_pinOutMap = _pinOutMap;
  n->_inputDummy = (DummyIO*)_inputDummy->clone();
  n->_outputDummy = (DummyIO*)_outputDummy->clone();
  n->_components[0] = (BaseComponent*)n->_inputDummy;
  n->_components[1] = (BaseComponent*)n->_outputDummy;

  // Give it the monitor object, but don't copy over the monitor points
  n->_monitor = _monitor;

  return (BaseComponent*)n;
}
