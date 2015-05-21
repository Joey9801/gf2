#ifndef gui_h
#define gui_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/splitter.h>
#include <wx/sizer.h>

#include "networkview.h"
#include "componentview.h"
#include "outputplot.h"

enum {
  ID_LoadNetwork = 1,
  ID_StartSimulation
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
    NetworkView *_netview;
    ComponentView *_compview;
    OutputPlot *_outputplot;

    wxString CurrentNetfilePath; // The Path to the network file we have open

    void OnLoadNetwork(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnCompSelect(wxTreeEvent& event);
};

#endif /*gui.h*/
