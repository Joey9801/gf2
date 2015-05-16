#ifndef NODETREE_H_
#define NODETREE_H_

#include <vector>
#include <string>

enum class NodeType { Base, Network, Gate };

struct NodeTreeBase {
  NodeTreeBase() {type = NodeType::Base;}
  NodeTreeBase(NodeType node_type) {type = node_type;}

  NodeType type;
  std::string name;

  // Input #0 is connected to node inputNodes[0], and has name inputNames[0]
  // Sim. for outputs.
  std::vector<unsigned int> inputNodes;
  std::vector<unsigned int> outputNodes;
  std::vector<std::string> inputNames;
  std::vector<std::string> outputNames;

  std::vector<NodeTreeBase*> children;
};

struct NodeTreeNetwork : public NodeTreeBase {
  NodeTreeNetwork() : NodeTreeBase(NodeType::Network) {}
};

struct NodeTreeGate : public NodeTreeBase {
  NodeTreeGate() : NodeTreeBase(NodeType::Gate) {}
  private:
    //Gate cannot have children
    std::vector<NodeTreeBase*> children;
};

#endif
