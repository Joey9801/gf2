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

//#include "../simulator/network.h"

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

  // The Path to the network file we have open
  wxString CurrentNetfilePath;
private:
  NetworkView *_netview;
  ComponentView *_compview;

  void OnLoadNetwork(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnCompSelect(wxTreeEvent& event);
};

enum
{
  ID_LoadNetwork = 1
};

#endif /*gui.h*/