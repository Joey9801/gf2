#ifndef NODETREE_H_
#define NODETREE_H_

#include <vector>
#include <string>
#include <wx/treebase.h>

enum class NodeType { Base, Network, Gate };

struct NodeTreeBase : public wxTreeItemData {
  NodeTreeBase() {type = NodeType::Base;}
  NodeTreeBase(NodeType node_type) {type = node_type;}

  NodeType type;

  // Name is the type of component (And, Nand etc..)
  // Nickname is given to a particular instance of a component
  // Nicknames are gauranteed to be unique within each subnetwork
  std::string name;
  std::string nickname;

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
