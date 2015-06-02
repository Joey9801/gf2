#include "nodetree.h"

NodeTree::NodeTree(NodeType node_type) :
  type(node_type),
  name(""),
  nickname(""),
  parent(NULL)
{
}

std::vector<std::string> NodeTree::getOutputSignature(unsigned int outputId) {
  //First check that the output exists
  if(outputId >= outputNames.size()) {
    LOG_ERROR << "outputId out of range";
    throw 1;
  }

  std::vector<std::string> signature;
  signature.push_back( outputNames[outputId] );
  signature.push_back( nickname );

  if(not parent)
    return signature;

  NodeTree * next = parent;
  while(next->parent) {
    signature.push_back( next->nickname );
    next = next->parent;
  }

  return signature;
}

std::vector<std::string> NodeTree::getOutputSignature(std::string outputName) {
  for(unsigned int i=0; i<outputNames.size(); i++) {
    if(outputName == outputNames[i])
      return getOutputSignature(i);
  }

  // We didn't find the outputName in the list, throw an error
  LOG_ERROR << "outputName not found as an output";
  throw 1;
}
