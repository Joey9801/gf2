#include "networkview.h"

//wxBEGIN_EVENT_TABLE(NetworkView, wxFrame)
//wxEND_EVENT_TABLE()

NetworkView::NetworkView(wxWindow *parent, wxWindowID id) 
  : wxPanel(parent, id)
{
  _treectrl = new wxTreeCtrl(this, -1);
  _treectrl->AddRoot(wxT("Network"));
  _treectrl->AppendItem(_treectrl->GetRootItem(), wxT("Component 1"));
  _treectrl->AppendItem(_treectrl->GetRootItem(), wxT("Component 2"));

  wxBoxSizer *nvsizer = new wxBoxSizer(wxVERTICAL);
  nvsizer->Add(_treectrl, 1,wxEXPAND,0);
  SetSizer(nvsizer);
}

void NetworkView::loadNetwork(NodeTreeBase *network)
{
  _network = network;
}

void NetworkView::displayNetwork()
{
  _treectrl->DeleteAllItems;
  _treectrl->AddRoot(_network.nickname);

  //add nodes for all components and subnetworks in network
  for(int i = 0; i < _network.children.size(), i++)
}

