#ifndef networkview_h
#define networkview_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/treectrl.h>

#include "../simulator/network.h"

class NetworkView: public wxTreeCtrl
{
public:
  NetworkView(wxWindow *parent, const wxWindowID id,
              const wxPoint& pos, const wxSize& size);
  virtual ~NetworkView(){};

private:
  //wxDECLARE_EVENT_TABLE();
};

#endif /*networkview.h*/
