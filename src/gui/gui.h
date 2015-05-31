#ifndef gui_h
#define gui_h

#include<vector>
#include<string>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/splitter.h>
#include <wx/sizer.h>

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

#include "networkview.h"
#include "componentview.h"
#include "outputplot.h"

#include "../structures/nodetree.h"
#include "../simulator/rootnetwork.h"
#include "../simulator/monitor.h"
#include "../parser/builder.h"

enum {
  ID_LoadNetwork = 1
};

class MyApp: public wxApp
{
  public:
    bool OnInit(); //automatically called when the application starts
  private:
};

class MyFrame: public wxFrame
{
  public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

  private:
    RootNetwork * _network;
    Monitor * _monitor;

    NetworkView *_netview;
    ComponentView *_compview;
    OutputPlot *_outputplot;

    wxString CurrentNetfilePath; // The Path to the network file we have open

    void OnLoadNetwork(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnCompSelect(wxTreeEvent& event);

    void OnToggleMonitor(wxCommandEvent &event);
};

#endif /*gui.h*/
