#ifndef componentview_h
#define componentview_h

#include <sstream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/imaglist.h>
#include <wx/renderer.h>

#include "../structures/nodetree.h"
#include "../simulator/monitor.h"
#include "../simulator/rootnetwork.h"

class ComponentView: public wxPanel
{
  public:
    ComponentView(wxWindow *parent, wxWindowID id=wxID_ANY);
    virtual ~ComponentView(){};

    void selectComponent(NodeTree * component);

    void setMonitor(Monitor * m);
    void setNetwork(RootNetwork * n);

    enum {
      ID_ToggleMonitor = 1,
      ID_ToggleInput
    };

  private:
    Monitor * _monitor;
    RootNetwork * _network;

    wxStaticText *_overview;
    wxListView *_listview;
    wxButton *_ToggleMonitorButton;
    wxButton *_ToggleInputButton;
    NodeTree *_component;

    bool IsMonitored(long item) const;
    void OnItemSelect(wxListEvent &event);
    void OnToggleMonitor(wxCommandEvent &event);
    void OnToggleInput(wxCommandEvent &event);
    void OnColWidthChanged(wxCommandEvent &event);
};

#endif /*componentview.h*/
