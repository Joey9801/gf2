#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "componentview.h"

ComponentView::ComponentView(wxWindow *parent, wxWindowID id)
  : wxPanel(parent, id)
{
  //Create Overview text field
  _overview = new wxStaticText(this, -1, _("Component name: ") + "\n" + _("Component type: "),
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
  col2.SetWidth(80);
  _listview->InsertColumn(2, col2);

  // Add fourth column
  wxListItem col3;
  col3.SetId(3);
  col3.SetText( _("Connected to") );
  col3.SetWidth(120);
  _listview->InsertColumn(3, col3);

  //Add the control buttons
  _ToggleMonitorButton = new wxButton(this, ID_ToggleMonitor, _("Toggle Monitor Point"));
  _ToggleMonitorButton->Disable();
  _ToggleInputButton = new wxButton(this, ID_ToggleInput, _("Switch Input State"));
  _ToggleInputButton->Disable();

  wxBoxSizer *cvsizer = new wxBoxSizer(wxVERTICAL);
  cvsizer->Add(_overview, 0,wxFIXED_MINSIZE | wxALL , 10);
  cvsizer->Add(_listview, 1,wxEXPAND,0);

  wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
  buttonsizer->Add(_ToggleMonitorButton);
  buttonsizer->Add(_ToggleInputButton);

  cvsizer->Add(buttonsizer, 0, wxFIXED_MINSIZE | wxTOP, 5);
  SetSizer(cvsizer);

  _listview->Bind(wxEVT_LIST_ITEM_SELECTED, &ComponentView::OnItemSelect, this);
  Bind(wxEVT_BUTTON, &ComponentView::OnToggleMonitor, this, ID_ToggleMonitor);
  Bind(wxEVT_BUTTON, &ComponentView::OnToggleInput, this, ID_ToggleInput);
  return;
}

void ComponentView::selectComponent(NodeTree *component) {
  _component = component;
  _listview->DeleteAllItems();


  _overview->SetLabel(_("Component name: ") + _component->nickname + "\n"
      + _("Component type: ") + _component->name);
  long itemIndex;

  //add all the outputs to the listview
  for(std::vector<std::string>::size_type i = _component->outputNames.size() - 1;
          i != (std::vector<int>::size_type) -1; i--) {
    //Add output name
    itemIndex = _listview->InsertItem(0, _("Output"));
    _listview->SetItem(itemIndex, 1, wxString(_component->outputNames[i]));
    if(IsMonitored(itemIndex)) _listview->SetItem(itemIndex, 2, wxString("Yes"));

    //Find out which inputs the outputs are connected to
    if(_component->parent != NULL){ //don't need this for root network, which was no parent
      wxString inputnamestr = "";
      //loop through all the components at this level of the network
      for(std::vector<NodeTree*>::iterator it = _component->parent->children.begin(); 
          it != _component->parent->children.end(); ++it) {
        //loop through all the inputs of the component
        for(std::vector<unsigned int>::size_type j = (*it)->inputNodes.size() - 1;
            j != (std::vector<unsigned int>::size_type) -1; j--) {
          if((*it)->inputNodes[j] == _component->outputNodes[i]){
            if(inputnamestr != "") inputnamestr += ", ";
            inputnamestr += (*it)->nickname + "." + (*it)->inputNames[j];
            _listview->SetItem(itemIndex, 3, inputnamestr);
          }
        }
      }
    }
  }
  //add all the inputs to the listview similarly
  for(std::vector<std::string>::size_type i = _component->inputNames.size() - 1;
          i != (std::vector<int>::size_type) -1; i--) {
    itemIndex = _listview->InsertItem(0, wxString("Input"));
    _listview->SetItem(itemIndex, 1, wxString(_component->inputNames[i]));

    if(_component->parent){ //If _component isnt the root network
      if(_component->inputNodes[i] == 0){
        _listview->SetItem(itemIndex, 3, _("Low (constant)"));
      }else if(_component->inputNodes[i] == 1){
        _listview->SetItem(itemIndex, 3, _("High (constant)"));
      }else {
        for(std::vector<NodeTree*>::iterator it = _component->parent->children.begin(); 
            it != _component->parent->children.end(); ++it) {
          for(std::vector<unsigned int>::size_type j = (*it)->outputNodes.size() - 1;
              j != (std::vector<unsigned int>::size_type) -1; j--) {
            if((*it)->outputNodes[j] == _component->inputNodes[i]){
              wxString outputnamestr = (*it)->nickname + "." + (*it)->outputNames[j];
              _listview->SetItem(itemIndex, 3, outputnamestr);
            }
          }
        }
      }
    } 
    else {//If we're on Root network
      if(_network->getInput(_component->inputNames[i])){
        _listview->SetItem(itemIndex, 3, _("High"));
      }
      else{
        _listview->SetItem(itemIndex, 3, _("Low"));
      }
    }
  }
  //deselect all items
  long item = -1;
  while ((item = _listview->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1){
    _listview->SetItemState(item, 0, wxLIST_MASK_STATE);
  }
  _ToggleMonitorButton->Disable();
  _ToggleInputButton->Disable();
}

void ComponentView::setMonitor(Monitor * m) {
  _monitor = m;
  return;
}
void ComponentView::setNetwork(RootNetwork * n) {
  _network = n;
  return;
}

bool ComponentView::IsMonitored(long item) const
{
  std::string outputName = _listview->GetItemText(item, 1).ToStdString();
  std::vector<std::string> signature = _component->getOutputSignature(outputName);

  try{
    _monitor->findPoint(signature);
  } catch(...){
    return false;
  }
  return true;
}

void ComponentView::OnItemSelect(wxListEvent &event)
{
  bool onlyoutputsselected = true;
  long item = -1;
  while ((item = _listview->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1){
    if(_listview->GetItemText(item, 0) == _("Input")) {
      onlyoutputsselected = false;
      break;
    }
  }
  if (onlyoutputsselected) _ToggleMonitorButton->Enable();
  else _ToggleMonitorButton->Disable();
  
  if(_component->parent == NULL){
    bool onlyinputsselected = true;
    long item = -1;
    while ((item = _listview->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1){
      if(_listview->GetItemText(item, 0) == _("Output")) {
        onlyinputsselected = false;
        break;
      }
    }
    if (onlyinputsselected) _ToggleInputButton->Enable();
    else _ToggleInputButton->Disable();
  }else _ToggleInputButton->Disable();
}

void ComponentView::OnToggleMonitor(wxCommandEvent &event)
{
  long item = -1;
  while ((item = _listview->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1){
    if(_listview->GetItemText(item, 0) == _("Output")) {
      if(not IsMonitored(item)) {
        std::string outputName = _listview->GetItemText(item, 1).ToStdString();
        std::vector<std::string> signature = _component->getOutputSignature(outputName);

        std::stringstream ss("");
        for(unsigned int i=signature.size()-1; i>0; i--)
          ss << signature[i] << ".";
        ss << signature[0];

        unsigned int pointId = _network->addMonitorPoint(signature);
        _monitor->renamePoint(pointId, ss.str());

        _listview->SetItem(item, 2, _("Yes"));
      }
      else {
        std::string outputName = _listview->GetItemText(item, 1).ToStdString();
        std::vector<std::string> signature = _component->getOutputSignature(outputName);

        std::stringstream ss("");
        for(unsigned int i=signature.size()-1; i>0; i--)
          ss << signature[i] << ".";
        ss << signature[0];

        _network->removeMonitorPoint(signature);
        _listview->SetItem(item, 2, wxString(""));
      }
    }
  }
  event.Skip();
}

void ComponentView::OnToggleInput(wxCommandEvent &event)
{
  long item = -1;
  while ((item = _listview->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1){
    std::string inputName = _listview->GetItemText(item, 1).ToStdString();
    bool next = not _network->getInput(inputName);

    _network->setInput(inputName, next);
    _listview->SetItem(item, 3, next ? _("High") : _("Low"));

  }
}
