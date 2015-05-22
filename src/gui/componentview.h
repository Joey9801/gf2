#ifndef componentview_h
#define componentview_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>

#include "../structures/nodetree.h"

class ComponentView: public wxPanel
{
  public:
    ComponentView(wxWindow *parent, wxWindowID id=wxID_ANY);
    virtual ~ComponentView(){};

    void selectComponent(NodeTreeBase *component);

  private:
    wxListView *_listview;
    NodeTreeBase *_component;
};

#endif /*componentview.h*/
