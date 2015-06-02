#include "gui.h"

// command line arguments handling
void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.AddParam(_("locale"),
                    wxCMD_LINE_VAL_STRING,
                    wxCMD_LINE_PARAM_OPTIONAL);

    wxApp::OnInitCmdLine(parser);
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if ( !wxApp::OnCmdLineParsed(parser) )
        return false;

    if ( parser.GetParamCount() )
    {
        const wxString loc = parser.GetParam();
        const wxLanguageInfo * const lang = wxLocale::FindLanguageInfo(loc);
        if ( !lang )
        {
            wxLogError(_("Locale \"%s\" is unknown."), loc);
            return false;
        }

        _lang = static_cast<wxLanguage>(lang->Language);
    }

    return true;
}

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    if ( _lang == wxLANGUAGE_UNKNOWN )
    {
        int lng = wxGetSingleChoiceIndex
                  (
                    _("Please choose language:"),
                    _("Language"),
                    WXSIZEOF(langNames),
                    langNames
                  );
        _lang = lng == -1 ? wxLANGUAGE_DEFAULT : langIds[lng];
    }

    // don't use wxLOCALE_LOAD_DEFAULT flag so that Init() doesn't return
    // false just because it failed to load wxstd catalog
    if ( !_locale.Init(_lang, wxLOCALE_DONT_LOAD_DEFAULT) )
    {
        wxLogWarning(_("This language is not supported by the system."));

        // continue nevertheless
    }

    // normally this wouldn't be necessary as the catalog files would be found
    // in the default locations, but when the program is not installed the
    // catalogs are in the build directory where we wouldn't find them by
    // default
    wxLocale::AddCatalogLookupPathPrefix("./i18n");

    // Initialize the catalogs we'll be using
    const wxLanguageInfo* pInfo = wxLocale::GetLanguageInfo(_lang);
    if (!_locale.AddCatalog("logicsim"))
    {
        wxLogError(_("Couldn't find/load the 'logicsim' catalog for locale '%s'."),
                   pInfo ? pInfo->GetLocaleName() : _("unknown"));
    }

    // Now try to add wxstd.mo so that loading "NOTEXIST.ING" file will produce
    // a localized error message:
    _locale.AddCatalog("wxstd");
        // NOTE: it's not an error if we couldn't find it!

    // this catalog is installed in standard location on Linux systems and
    // shows that you may make use of the standard message catalogs as well
    //
    // if it's not installed on your system, it is just silently ignored
#ifdef __LINUX__
    {
        wxLogNull noLog;
        _locale.AddCatalog("fileutils");
    }
#endif

    //Initialise the logger
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    // Create the main frame window
    MyFrame *frame = new MyFrame(_locale);

    frame->SetMinSize( wxSize(800, 600) );

    // Show the frame
    frame->Show(true);

    return true;
}
/*
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
*/

MyFrame::MyFrame(wxLocale& locale)
  : wxFrame(NULL, wxID_ANY, _("Logic Simulator"), wxPoint(500, 50), wxSize(800, 100)),
    _locale(locale)
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

  _network = NULL;
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
           Joe Roberts, Duncan Barber and Daniel Potter\n\n\n")
      + wxString::FromUTF8("\u0647\u0630\u0627 \u0647\u0648 \u0645\u062d\u0627\u0643\u0627\u0629 \u0627\u0644\u0645\u0646\u0637\u0642 \u0648\u0636\u0639\u062a \u0644\u0644\u0645\u0634\u0631\u0648\u0639 GF2 \u0641\u064a \u0645\u0644\u0642\u0646 \u062c\u0648 \u0631\u0648\u0628\u0631\u062a\u0633\u060c \u062f\u0646\u0643\u0627\u0646 \u0628\u0627\u0631\u0628\u0631 \u0648\u062f\u0627\u0646\u064a\u0627\u0644 \u0628\u0648\u062a\u0631"),
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
