#include "gui.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(ID_LoadNetwork, MyFrame::OnLoadNetwork)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

bool MyApp::OnInit()
{
  MyFrame *frame = new MyFrame( "Logic Simulator", wxPoint(50, 50), wxSize(450, 340) );
  frame->Show( true );
  return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, wxID_ANY, title, pos, size)
{
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_LoadNetwork, "&Open Network File\tCtrl-O",
  "Load a file defining the logic network");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuHelp, "&Help" );
  SetMenuBar( menuBar );
  CreateStatusBar();
  SetStatusText( "" );

  wxPanel *panel = new wxPanel(this, -1);
  
  NetworkView *myview = new NetworkView(panel, wxID_ANY, wxPoint(10,10), wxSize(300,300));
  (void)myview;
}

void MyFrame::OnExit(wxCommandEvent& event)
{
  Close( true );
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
  wxMessageBox( "This is a logic simulator developed for project GF2 at CUED \
by Joe Roberts, Duncan Barber and Daniel Potter",
  "About this program", wxOK | wxICON_INFORMATION );
}

void MyFrame::OnLoadNetwork(wxCommandEvent& event)
{
  wxFileDialog* OpenDialog = new wxFileDialog(
  this, _("Choose a file to open"), wxEmptyString, wxEmptyString, 
  _("Network definition files (*.def)|*.def"),
  wxFD_OPEN, wxDefaultPosition);

// Creates a "open file" dialog
  if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
  {
    CurrentNetfilePath = OpenDialog->GetPath();
  }

  // Clean up after ourselves
  OpenDialog->Destroy();
}
