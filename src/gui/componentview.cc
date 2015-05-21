#include "componentview.h"
#include <wx/treectrl.h>

ComponentView::ComponentView(wxWindow *parent, wxWindowID id)
  : wxPanel(parent, id)
{
  // Create list view
  m_listview = new wxListView(this, -1);

  // Add first column
  wxListItem col0;
  col0.SetId(0);
  col0.SetText( _("Property") );
  col0.SetWidth(100);
  m_listview->InsertColumn(0, col0);

  // Add second column
  wxListItem col1;
  col1.SetId(1);
  col1.SetText( _("Value") );
  col1.SetWidth(100);
  m_listview->InsertColumn(1, col1);

  long itemIndex = m_listview->InsertItem(0, "Type:"); //want this for col. 1
  m_listview->SetItem(itemIndex, 1, "NAND Gate"); //want this for col. 2
  itemIndex = m_listview->InsertItem(0, "Type2:"); //want this for col. 1
  m_listview->SetItem(itemIndex, 1, wxVERSION_STRING); //want this for col. 2

  wxBoxSizer *cvsizer = new wxBoxSizer(wxVERTICAL);
  cvsizer->Add(m_listview, 1,wxEXPAND,0);
  SetSizer(cvsizer);

  return;
}

void ComponentView::selectComponent(NodeTreeBase *component) {
  _component = component;

  return;
}

void showComponent() {
  return;
}

void ComponentView::SetComponent(wxString name) {
  long itemIndex = m_listview->InsertItem(0, "New Event");
  m_listview->SetItem(itemIndex, 1, name);

  return;
}
