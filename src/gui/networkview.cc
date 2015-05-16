#include "networkview.h"

//wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
//wxEND_EVENT_TABLE()

NetworkView::NetworkView(wxWindow *parent, wxWindowID id) 
  : wxPanel(parent, id)
{
  m_treectrl = new wxTreeCtrl(this, -1);
  m_treectrl->AddRoot(wxT("Network"));
  m_treectrl->AppendItem(m_treectrl->GetRootItem(), wxT("Component 1"));
  m_treectrl->AppendItem(m_treectrl->GetRootItem(), wxT("Component 2"));

  wxBoxSizer *nvsizer = new wxBoxSizer(wxVERTICAL);
  nvsizer->Add(m_treectrl, 1,wxEXPAND,0);
  SetSizer(nvsizer);
}

void NetworkView::loadNetwork(Network *network)
{
  _network = network;
}
