#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "componentview.h"

ComponentView::ComponentView(wxWindow *parent, wxWindowID id)
  : wxPanel(parent, id)
{
  //Create Overview text field
  _overview = new wxStaticText(this, -1, "Component name:\nComponent type:",
      wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

  // Create list view
  _listview = new wxListView(this, -1);

  // Add first column       
  wxListItem col0;
  col0.SetId(0);
  col0.SetText( _("I/O") );
  col0.SetWidth(100);
  _listview->InsertColumn(0, col0);

  // Add second column
  wxListItem col1;
  col1.SetId(1);
  col1.SetText( _("Name") );
  col1.SetWidth(100);
  _listview->InsertColumn(1, col1);

  // Add third column
  wxListItem col2;
  col2.SetId(2);
  col2.SetText( _("Monitored") );
  col2.SetWidth(100);
  _listview->InsertColumn(2, col2);

  // Add fourth column
  wxListItem col3;
  col3.SetId(3);
  col3.SetText( _("Connected to") );
  col3.SetWidth(100);
  _listview->InsertColumn(3, col3);

  //Add the control buttons
  _ToggleMonitorButton = new wxButton(this, -1, "Toggle Monitor Point");
  _ToggleMonitorButton->Disable();

  wxBoxSizer *cvsizer = new wxBoxSizer(wxVERTICAL);
  cvsizer->Add(_overview, 0,wxFIXED_MINSIZE | wxALL , 10);
  cvsizer->Add(_listview, 1,wxEXPAND,0);
  cvsizer->Add(_ToggleMonitorButton, 0, wxFIXED_MINSIZE | wxTOP, 5);
  SetSizer(cvsizer);

  _listview->Bind(wxEVT_LIST_ITEM_SELECTED, &ComponentView::OnItemSelect, this);
  _ToggleMonitorButton->Bind(wxEVT_BUTTON, &ComponentView::OnToggleMonitor, this);
  return;
}

void ComponentView::selectComponent(NodeTreeBase *component) {
  _component = component;
  _listview->DeleteAllItems();


  _overview->SetLabel("Component name: " + _component->nickname + "\nComponent type: " + _component->name);
  long itemIndex;
  for(std::vector<int>::size_type i = _component->outputNames.size() - 1; 
          i != (std::vector<int>::size_type) -1; i--) {
    itemIndex = _listview->InsertItem(0, wxString("Output"));
    _listview->SetItem(itemIndex, 1, wxString(_component->outputNames[i]));
  }
  for(std::vector<int>::size_type i = _component->inputNames.size() - 1; 
          i != (std::vector<int>::size_type) -1; i--) {
    itemIndex = _listview->InsertItem(0, wxString("Input"));
    _listview->SetItem(itemIndex, 1, wxString(_component->inputNames[i]));
  }
}

bool ComponentView::IsMonitored(long item) const
{
  if(_listview->GetItemText(item, 2) ==  "Yes"){
    return true;
  }else{
    return false;
  }
}

void ComponentView::OnItemSelect(wxListEvent &event)
{
  _ToggleMonitorButton->Enable();
}

void ComponentView::OnToggleMonitor(wxCommandEvent &event)
{
  long item = -1;
  while ((item = _listview->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1){
    _listview->SetItem(item, 2, (IsMonitored(item) ? wxString("") : wxString("Yes")));
  }
}
