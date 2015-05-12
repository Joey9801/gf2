#include "network.h"

Network::Network(void) :
  BaseComponent("Network", 0, 0)
{
  _componentMap["And"] = &createComponent<AndGate>;
  _componentMap["Nand"] = &createComponent<NandGate>;
  _componentMap["Or"] = &createComponent<OrGate>;
  _componentMap["Nor"] = &createComponent<NorGate>;
  _componentMap["Xor"] = &createComponent<XorGate>;
  _componentMap["DType"] = &createComponent<DType>;

  //Make room for the const values
  _nodesA.resize(2);
  _nodesB.resize(2);
}

Network::~Network() {}

//returns componentId
unsigned int Network::addComponent(std::string type) {
  if(_componentMap.find(type) == _componentMap.end()) {
    //TODO report an error
    return 0;
  }

  unsigned int componentId = _components.size();
  BaseComponent * c = _componentMap[type]();
  for(unsigned int i=0; i<c->numOutputs(); i++)
    c->setOutput(i, _nodesA.size()+i);
  
  _components.push_back(c);
  _nodesA.resize(_nodesA.size() + c->numOutputs(), false);
  _nodesB.resize(_nodesA.size());

  return componentId;
}

//Returns the inputId
unsigned int Network::addInput(void) {
  unsigned int inputId = _inputs.size();
  _inputs.push_back(0);
  _inputMap.push_back(_nodesA.size());
  _nodesA.push_back(false);
  _nodesB.push_back(false);

  return inputId;
}

unsigned int Network::addOutput(void) {
  unsigned int outputId = _outputs.size();
  _outputMap.push_back(0);
  _outputs.push_back(0);
  return outputId;
}

//Connect a member component to another member component
void Network::connect(unsigned int idOut, unsigned int pinOut, unsigned int idIn, unsigned int pinIn) {
  unsigned int node = _components[idOut]->getOutputNode(pinOut);
  _components[idIn]->setInput(pinIn, node);
  return;
}

//Connect a member component input to a network input
void Network::connectInput(unsigned int inputId, unsigned int idIn, unsigned int pinIn) {
  unsigned int node = _inputMap[inputId];
  _components[idIn]->setInput(pinIn, node);
  return;
}

//Connect a member component output to a network output
void Network::connectOutput(unsigned int outputId, unsigned int idOut, unsigned int pinOut) {
  unsigned int node = _components[idOut]->getOutputNode(pinOut);
  _outputMap[outputId] = node;
  return;
}

void Network::step(std::vector<bool>& a, std::vector<bool>& b) {

  //Copy inputs into node list
  for(unsigned int i=0; i<_inputs.size(); i++)
    _nodesA[_inputMap[i]] = a[_inputs[i]];

  _nodesA[0] = false;
  _nodesA[1] = true;

  for(std::vector<BaseComponent*>::iterator c = _components.begin();
        c != _components.end();
        c++)
    (*c)->step(_nodesA, _nodesB);

  _nodesA.swap(_nodesB);

  _nodesA[0] = false;
  _nodesA[1] = true;

  //Export the outputs
  for(unsigned int i=0; i<_outputs.size(); i++)
    b[_outputs[i]] = _nodesA[_outputMap[i]];

  return;
}

unsigned int Network::countComponents(void) {
  return _components.size();
}
