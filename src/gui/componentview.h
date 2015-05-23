#ifndef componentview_h
#define componentview_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/imaglist.h>
#include <wx/renderer.h>

#include "../structures/nodetree.h"

class ComponentView: public wxPanel
{
  public:
    ComponentView(wxWindow *parent, wxWindowID id=wxID_ANY);
    virtual ~ComponentView(){};

    void selectComponent(NodeTreeBase *component);

  private:
    wxStaticText *_overview;
    wxListView *_listview;
    wxButton *_ToggleMonitorButton;
    NodeTreeBase *_component;

    bool IsMonitored(long item) const;
    void OnItemSelect(wxListEvent &event);
    void OnToggleMonitor(wxCommandEvent &event);
};

#endif /*componentview.h*/
