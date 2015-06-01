#include "gui.h"

bool MyApp::OnInit() {
  //Initialise the logger
  static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::info, &consoleAppender);

  //Create the main window
  MyFrame *frame = new MyFrame( _("Logic Simulator"), wxPoint(500, 50), wxSize(800, 100) );
  frame->SetMinSize( wxSize(800, 600) );
  frame->Show( true );

  return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, wxID_ANY, title, pos, size)
{
  //Create menus and attach them to menu bar
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_LoadNetwork, _("&Open Network File\tCtrl-O"),
    _("Load a file defining the logic network"));

  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(ID_ShowErrors, _("Show &Errors\tCtrl-E"), _("Show all errors"));
  menuFile->AppendSeparator();
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));
  menuBar->Append(menuHelp, _("&Help"));
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
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnShowErrors, this, ID_ShowErrors);
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnAbout, this, wxID_ABOUT);

  //Events from Panes
  _netview->Bind(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, &MyFrame::OnCompSelect, this);
  Bind(wxEVT_BUTTON, &MyFrame::OnToggleMonitor, this, ComponentView::ID_ToggleMonitor);

  _monitor = new Monitor();
  _outputplot->setMonitor(_monitor);
  _compview->setMonitor(_monitor);

}

void MyFrame::OnCompSelect(wxTreeEvent& event)
{
  wxTreeItemData *selectednode = _netview->_treectrl->GetItemData(event.GetItem());
  NodeTree *node=dynamic_cast<NodeTree*>(selectednode);
  if (node == NULL){
    LOG_WARNING << "Invalid conversion of wxTreeItemData to NodeTreeBase";
    return;
  }
  _compview->selectComponent(node);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
  wxMessageBox(

_("This is a logic simulator developed for project GF2 at CUED by\n\
           Joe Roberts, Duncan Barber and Daniel Potter"),

      _("About this program"), wxOK | wxICON_INFORMATION );

  return;
}

void MyFrame::OnLoadNetwork(wxCommandEvent& event) {
  wxFileDialog* OpenDialog = new wxFileDialog(
      this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
      _("Network definition files (*.def)|*.def"),
      wxFD_OPEN, wxDefaultPosition);

  // Creates a "open file" dialog
  if (OpenDialog->ShowModal() == wxID_OK) { // if the user click "Open" instead of "Cancel"
    CurrentNetfilePath = OpenDialog->GetPath();

    RootNetwork * net;
    try {
      net = Builder::buildRoot( CurrentNetfilePath.ToStdString() );
    }
    catch(...) {
      LOG_ERROR << "Failed to build the network";
      return;
    }
    LOG_DEBUG << "Network built successfully";

    delete _network;
    delete _monitor;
    _network = net;
    _monitor = net->getMonitor();
    _network->setMonitor(_monitor);
    _compview->setNetwork(_network);
    _compview->setMonitor(_monitor);
    _outputplot->setMonitor(_monitor);
    _outputplot->setNetwork(_network);

    LOG_DEBUG << "About to load the network view";
    _netview->loadNetwork(_network->getNodeTree());
    _compview->selectComponent(_network->getNodeTree());

    LOG_DEBUG << "About to enable the simulation toolbar";
    _outputplot->EnableToolbar(true);
  }

  // Clean up after ourselves
  OpenDialog->Destroy();

  return;
}

void MyFrame::OnShowErrors(wxCommandEvent& event) {
  ErrorDialog* dlg = new ErrorDialog(this, -1, _("Errors and Warnings"));

  // Creates a "open file" dialog
  if (dlg->ShowModal() == wxID_OK) { // if the user click "Open" instead of "Cancel"
  }

  // Clean up after ourselves
  dlg->Destroy();

  return;
}
void MyFrame::OnToggleMonitor(wxCommandEvent &event)
{
  _outputplot->refresh();
}
