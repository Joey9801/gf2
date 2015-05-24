#include "networkview.h"

NetworkView::NetworkView(wxWindow *parent, wxWindowID id)
  : wxPanel(parent, id)
{
  _treectrl = new wxTreeCtrl(this, -1);

  wxBoxSizer *nvsizer = new wxBoxSizer(wxVERTICAL);
  nvsizer->Add(_treectrl, 1,wxEXPAND,0);
  SetSizer(nvsizer);
}

void NetworkView::loadNetwork(NodeTree *root) {
  _treectrl->DeleteAllItems();

  wxTreeItemId nodeId = _treectrl->AddRoot("Root network", -1, -1, root);

  for(std::vector<NodeTree*>::iterator it = root->children.begin();
      it != root->children.end();
      it++)
    recursive_addNode(nodeId, (*it));

  return;
}

void NetworkView::recursive_addNode(wxTreeItemId parentid, NodeTree *node) {

  wxTreeItemId nodeId = _treectrl->AppendItem(parentid, node->nickname,-1, -1, node);

  for(std::vector<NodeTree*>::iterator it = node->children.begin();
      it != node->children.end();
      it++)
    recursive_addNode(nodeId, (*it));
}
