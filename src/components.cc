#include "simulator.h"

BaseComponent::BaseComponent(std::string name, std::vector<unsigned int> nodes) :
    _name(name)
{
  _nodes = nodes;
}

BaseComponent::~BaseComponent(void)
{
}

std::string BaseComponent::getName(void) {
  return _name;
}

std::vector<unsigned int> BaseComponent::getNodes() {
  return _nodes;
}

void AndGate::step(std::vector<bool>& in, std::vector<bool>& out) {
  out[_nodes[2]] = in[_nodes[0]] & in[_nodes[1]];
  return;
}

void OrGate::step(std::vector<bool>& in, std::vector<bool>& out) {
  out[_nodes[2]] = in[_nodes[0]] | in[_nodes[1]];
  return;
}

void XorGate::step(std::vector<bool>& in, std::vector<bool>& out) {
  out[_nodes[2]] = in[_nodes[0]] ^ in[_nodes[1]];
  return;
}

void NandGate::step(std::vector<bool>& in, std::vector<bool>& out) {
  out[_nodes[2]] = !(in[_nodes[0]] & in[_nodes[1]]);
  return;
}
