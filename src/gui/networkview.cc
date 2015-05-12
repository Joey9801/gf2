#include "gui.h"

//wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
//wxEND_EVENT_TABLE()

NetworkView::NetworkView(wxWindow *parent, const wxWindowID id,
              const wxPoint& pos, const wxSize& size)
  : wxTreeCtrl(parent, id, pos, size)
{
  AddRoot(wxT("Network"));
  AppendItem(GetRootItem(), wxT("Component 1"));
  AppendItem(GetRootItem(), wxT("Component 2"));
}
