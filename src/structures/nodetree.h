#ifndef NODETREE_H_
#define NODETREE_H_

#include <vector>
#include <string>
#include <wx/treebase.h>

enum class NodeType { Network, Gate };

struct NodeTree : public wxTreeItemData {
  NodeTree(NodeType node_type) {type = node_type;}

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

  NodeTree * parent;
  std::vector<NodeTree*> children;
};

#endif
