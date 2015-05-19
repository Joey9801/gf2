#include "networkview.h"

NetworkView::NetworkView(wxWindow *parent, wxWindowID id) 
  : wxPanel(parent, id)
{
  _treectrl = new wxTreeCtrl(this, -1);

  wxBoxSizer *nvsizer = new wxBoxSizer(wxVERTICAL);
  nvsizer->Add(_treectrl, 1,wxEXPAND,0);
  SetSizer(nvsizer);
}

void NetworkView::loadNetwork(NodeTreeBase *network)
{
  _network = network;
  displayNetwork();
}

void NetworkView::displayNetwork()
{
  _treectrl->DeleteAllItems();
  recursive_addNode(0, _network);
}

void NetworkView::recursive_addNode(wxTreeItemId parentid, NodeTreeBase *node)
{
  if (node->type == NodeType::Gate){
    _treectrl->AppendItem(parentid, node->nickname);
  }else{
    wxTreeItemId nodeid;
    if(node->type == NodeType::Base){
      nodeid = _treectrl->AddRoot("Network");
    }else{
      nodeid = _treectrl->AppendItem(parentid, node->nickname);
    }
    for (unsigned int i = 0; i < node->children.size(); i++){
      recursive_addNode(nodeid, node->children[i]);
    }
  }
}
