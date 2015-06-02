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

  wxTreeItemId nodeId = _treectrl->AddRoot(root->nickname, -1, -1, root);

  try {
    for(std::vector<NodeTree*>::iterator it = root->children.begin();
        it != root->children.end();
        it++)
      recursive_addNode(nodeId, (*it));
  }
  catch(...) {
    LOG_ERROR << "Something went wrong with recursively building the NodeTree - Tell Joe what you did to make this happen";
  }

  return;
}

void NetworkView::recursive_addNode(wxTreeItemId parentid, NodeTree *node) {
  if (node->nickname!="inputs" and node->nickname!="outputs"){
    wxTreeItemId nodeId = _treectrl->AppendItem(parentid, node->nickname,-1, -1, node);

    for(std::vector<NodeTree*>::iterator it = node->children.begin();
        it != node->children.end();
        it++)
      recursive_addNode(nodeId, (*it));
  }
}
