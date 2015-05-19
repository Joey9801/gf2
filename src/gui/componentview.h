#ifndef componentview_h
#define componentview_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <wx/sizer.h>

#include "../simulator/basecomponent.h"

class ComponentView: public wxPanel
{
public:
  ComponentView(wxWindow *parent, wxWindowID id=wxID_ANY);
  virtual ~ComponentView(){};
  
  void selectComponent(BaseComponent *component);

  wxListView *m_listview;
private:
  BaseComponent *_component;
  //wxDECLARE_EVENT_TABLE();
};

#endif /*componentview.h*/
