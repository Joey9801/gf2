#include "gui.h"

bool MyApp::OnInit() {
  //Initialise the logger
  static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::debug, &consoleAppender);

  //Create the main window
  MyFrame *frame = new MyFrame( "Logic Simulator", wxPoint(500, 50), wxSize(800, 100) );
  frame->SetMinSize( wxSize(800, 600) );
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
  menuSim->Append(ID_StartSimulation, "&Run Simulation\tCtrl-R", "Start the Simulation");
  menuSim->Enable(ID_StartSimulation, false);

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
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnRunSimulation, this, ID_StartSimulation);
  Bind(wxEVT_BUTTON, &MyFrame::OnRunSimulation, this, ID_RunSim);

  //Events from Panes
  _netview->Bind(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, &MyFrame::OnCompSelect, this);

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

"This is a logic simulator developed for project GF2 at CUED by\n\
           Joe Roberts, Duncan Barber and Daniel Potter",

      "About this program", wxOK | wxICON_INFORMATION );

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

    Network * net;
    try {
      net = Builder::build( CurrentNetfilePath.ToStdString() );
    }
    catch(...) {
      LOG_ERROR << "Failed to build the network";
      return;
    }
    LOG_DEBUG << "Network built successfully";

    delete _network;
    delete _monitor;
    _monitor = new Monitor();
    _network = net;
    _network->setMonitor(_monitor);
    _compview->setNetwork(_network);
    _compview->setMonitor(_monitor);
    _outputplot->setMonitor(_monitor);

    _netview->loadNetwork(_network->getNodeTree());
    _compview->selectComponent(_network->getNodeTree());

    GetMenuBar()->Enable(GetMenuBar()->FindMenuItem("Simulation", "Run Simulation"), true);
  }

  // Clean up after ourselves
  OpenDialog->Destroy();

  return;
}

void MyFrame::OnRunSimulation(wxCommandEvent& event) {
  //Create a nodelist, since we're not yet using the RootNetwork object
  unsigned int numNodes = _network->numInputs() + _network->numOutputs();
  std::vector<bool> nodes(numNodes, false);

  long numberofsteps = wxGetNumberFromUser("Enter number of steps to simulate:",
      "Steps", "Setup Simulation", 10, 1, 1000);
  //Run for a fixed 50 cycles for the moment
  for(unsigned int i=0; i<numberofsteps; i++)
    _network->step(nodes, nodes);

  _outputplot->refresh();

  return;
}
