#ifndef gui_h
#define gui_h

#include<vector>
#include<string>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/cmdline.h>
#include <wx/intl.h>
#include <wx/splitter.h>
#include <wx/sizer.h>

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

#include "networkview.h"
#include "componentview.h"
#include "outputplot.h"
#include "errordialog.h"

#include "../structures/nodetree.h"
#include "../simulator/rootnetwork.h"
#include "../simulator/monitor.h"
#include "../parser/builder.h"
#include "../errors/error.h"

class MyApp: public wxApp
{
public:
    MyApp() { _lang = wxLANGUAGE_UNKNOWN; }

    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
    virtual bool OnInit();

protected:
    wxLanguage _lang;  // language specified by user
    wxLocale _locale;  // locale we'll be using
};

// language data
static const wxLanguage langIds[] =
{
    wxLANGUAGE_DEFAULT,
    wxLANGUAGE_GERMAN,
    wxLANGUAGE_ENGLISH_US
};

// note that it makes no sense to translate these strings, they are
// shown before we set the locale anyhow
const wxString langNames[] =
{
    "System default",
    "German",
    "English"
};

class MyFrame: public wxFrame
{
  public:
    MyFrame(wxLocale& locale);

  enum {
    ID_LoadNetwork = 1,
    ID_ShowErrors
  };

  private:
    wxLocale& _locale;  // locale we'll be using

    RootNetwork * _network;
    Monitor * _monitor;
    ErrorList * _errorList;

    NetworkView *_netview;
    ComponentView *_compview;
    OutputPlot *_outputplot;

    wxString CurrentNetfilePath; // The Path to the network file we have open

    void OnLoadNetwork(wxCommandEvent& event);
    void OnShowErrors(wxCommandEvent& event);
    void ShowErrors(void);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnCompSelect(wxTreeEvent& event);

    void OnToggleMonitor(wxCommandEvent &event);
};

#endif /*gui.h*/
