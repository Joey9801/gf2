#include "gui.h"

bool MyApp::OnInit()
{
  MyFrame *frame = new MyFrame( "Logic Simulator", wxPoint(50, 50), wxSize(800, 600) );
  frame->Show( true );
  return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, wxID_ANY, title, pos, size)
{
  //Create menus and attach them to menu bar
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_LoadNetwork, "&Open Network File\tCtrl-O",
  "Load a file defining the logic network");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);
  wxMenu *menuSim = new wxMenu;
  menuSim->Append(ID_StartSimulation, "&Run Simulation", "Start the Simulation");
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");
  menuBar->Append(menuSim, "&Simulation");
  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText("");

  //Create Main Spitterwindow, which will be split horizontally, containing
  //another splitterwindow and the Outputplot
  wxSplitterWindow *splittermain = new wxSplitterWindow(this, wxID_ANY);

  //Sizer for top splitterwindow
  wxBoxSizer *sizertop = new wxBoxSizer(wxVERTICAL);
  //Create Top Spitterwindow, which will be split vertically, containing the
  //networkview and componentview
  wxSplitterWindow *splittertop = new wxSplitterWindow(splittermain, wxID_ANY);
  splittertop->SetSashGravity(0.5); //both panes in sw resize equally
  splittertop->SetMinimumPaneSize(20); // Smallest size of pane allowed
  sizertop->Add(splittertop, 1,wxEXPAND,0);

  _netview = new NetworkView(splittertop, wxID_ANY);
  _compview = new ComponentView(splittertop, wxID_ANY);  

  splittertop->SplitVertically(_netview, _compview);
  splittermain->SetSizer(sizertop);
  sizertop->SetSizeHints(this);

  _outputplot = new OutputPlot(splittermain, wxID_ANY);

  wxBoxSizer *sizermain = new wxBoxSizer(wxVERTICAL);
  sizermain->Add(splittermain, 1,wxEXPAND,0);
  splittermain->SetSashGravity(0.5); //both panes in sw resize equally
  splittermain->SetMinimumPaneSize(20); // Smallest size of pane allowed
  splittermain->SplitHorizontally(splittertop, _outputplot);
  this->SetSizer(sizermain);
  sizermain->SetSizeHints(this);

  //Bind event handlers to events dynamically:
  //Events from Menubar
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnLoadNetwork, this, ID_LoadNetwork);
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnAbout, this, wxID_ABOUT);
  //Events from Panes
  _netview->Bind(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, &MyFrame::OnCompSelect, this);

}

void MyFrame::OnCompSelect(wxTreeEvent& event)
{
  std::vector<wxString> componentpath;
  wxTreeItemId node = event.GetItem();
  do{
    componentpath.push_back(_netview->_treectrl->GetItemText(node));
    node = _netview->_treectrl->GetParent();
  } while ( node != _netview->_treectrl->GetRootItem());
  NodeTreeBase *_selectednode;
  for (unsigned int i = componentpath.size()-1; i >=0 ; i--){
    for (unsigned int j = 0; j < _nodetree->children.size(); j++){
      if (wxString(_nodetree->children[j]->nickname) == componentpath[i]){
        _selectednode = _nodetree->children[j];
        break;
      }
    }
  }
  _compview->selectComponent(_nodetree);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
  wxMessageBox( "This is a logic simulator developed for project GF2 at CUED by\
      Joe Roberts, Duncan Barber and Daniel Potter",
      "About this program", wxOK | wxICON_INFORMATION );
}

void MyFrame::OnLoadNetwork(wxCommandEvent& event)
{
  wxFileDialog* OpenDialog = new wxFileDialog(
      this, _("Choose a file to open"), wxEmptyString, wxEmptyString, 
      _("Network definition files (*.def)|*.def"),
      wxFD_OPEN, wxDefaultPosition);

  // Creates a "open file" dialog
  if (OpenDialog->ShowModal() == wxID_OK){ // if the user click "Open" instead of "Cancel"
    CurrentNetfilePath = OpenDialog->GetPath();
  }

  // Clean up after ourselves
  OpenDialog->Destroy();
}
