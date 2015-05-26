#include "rootnetwork.h"

RootNetwork::RootNetwork(Network * net)
{
  LOG_DEBUG << "Creating RootNetwork from Network";
  numConnections = net->numConnections;

  _definition = net->_definition;

  _components = net->_components;
  _componentNames = net->_componentNames;

  _nodesA = net->_nodesA;
  _nodesB = net->_nodesB;

  _inputs = net->_inputDummy->_outputs;
  _inputVals = std::vector<bool>(_inputs.size(), false);
  _outputs = net->_outputDummy->_inputs;
  _inputVectors = net->_inputVectors;
  _outputVectors = net->_outputVectors;

  _pinInMap = net->_pinInMap;
  _pinOutMap = net->_pinOutMap;

  // Note that we don't copy the IO dummies
  // This is because a RootNetwork is NOT a component contained in a higher network

  _monitor = net->_monitor;
  _monitorPoints = net->_monitorPoints;

  _time = 0;
  _rate = net->_rate;
  _async = net->_async;
}

RootNetwork::~RootNetwork() {}

void RootNetwork::step(void) {
  LOG_VERBOSE;
  LOG_DEBUG;
  _nodesA[0] = false;
  _nodesA[1] = true;

  for(unsigned int i=0; i<_inputs.size(); i++)
    _nodesA[_inputs[i]] = _inputVals[i];

  LOG_DEBUG;
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

  LOG_DEBUG;
  _nodesA[0] = false;
  _nodesA[1] = true;

  for(std::map<unsigned int, unsigned int>::iterator it = _monitorPoints.begin();
      it != _monitorPoints.end();
      it++) {
    _monitor->setValue( it->first, _time, _nodesA[it->second]);
    LOG_DEBUG << "(time, value) = (" << _time << ", " << _nodesA[it->second] << ")";
  }

  _time++;

}

void RootNetwork::setInput(unsigned int inputId, bool value) {
  _inputVals[inputId] = value;
  LOG_DEBUG << "input " << inputId << " set to " << value;
  return;
}
void RootNetwork::setInput(std::string inputName, bool value) {
  if(_pinInMap.find(inputName) == _pinInMap.end()) {
    // TODO raise an error
    throw 1;
  }
  unsigned int inputId = _pinInMap[inputName];
  setInput(inputId, value);
  return;
}
bool RootNetwork::getInput(unsigned int inputId) {
  return _inputVals[inputId];
}
bool RootNetwork::getInput(std::string inputName) {
  if(_pinInMap.find(inputName) == _pinInMap.end()) {
    // TODO raise an error
    throw 1;
  }
  unsigned int inputId = _pinInMap[inputName];
  return getInput(inputId);
}

bool RootNetwork::getOutput(unsigned int outputId) {
  unsigned int node = _outputs[outputId];
  return _nodesA[node];
}
bool RootNetwork::getOutput(std::string outputName) {
  if(_pinOutMap.find(outputName) == _pinOutMap.end() ) {
    // TODO raise an error
    throw 1;
  }
  unsigned int outputId = _pinOutMap[outputName];
  return getOutput(outputId);
}

NodeTree * RootNetwork::getNodeTree(void) {
  LOG_DEBUG;
  NodeTree * n = new NodeTree(NodeType::Network);

  n->name = _name;
  n->nickname = "Root network";

  LOG_DEBUG;

  for(unsigned int i=0; i<_components.size(); i++) {
    n->children.push_back( _components[i]->getNodeTree() );
    n->children.back()->parent = n;
  }

  LOG_DEBUG;
  for(pin_map::iterator it = _componentNames.begin();
      it != _componentNames.end();
      it ++) {
    n->children[(*it).second]->nickname = (*it).first;
  }
  LOG_DEBUG;

  n->inputNodes = _inputs;
  n->outputNodes = _outputs;
  LOG_DEBUG;

  n->inputNames.resize(_pinInMap.size());
  for(pin_map::iterator it=_pinInMap.begin(); it != _pinInMap.end(); it++)
    n->inputNames[(*it).second] = (*it).first;
  LOG_DEBUG;

  n->outputNames.resize(_pinOutMap.size());
  for(pin_map::iterator it=_pinOutMap.begin(); it != _pinOutMap.end(); it++)
    n->outputNames[(*it).second] = (*it).first;
  LOG_DEBUG;

  return n;
}

unsigned int RootNetwork::addMonitorPoint(std::vector<std::string>& signature) {
  if(signature.size()==2) {
    LOG_DEBUG << signature[0] << ", " << signature[1];
    if(signature[1] == "Root network") {
      unsigned int pointId = _monitor->addPoint(signature);
      LOG_DEBUG;
      unsigned int outputId = _pinOutMap.find(signature[0])->second;
      LOG_DEBUG << "outputId " << outputId;
      unsigned int node = _outputs[outputId];
      LOG_DEBUG;
      _monitorPoints[pointId] = node;
      LOG_DEBUG;
      return pointId;
    }
  }
  return Network::addMonitorPoint(signature, 0);
}
