#ifndef networkview_h
#define networkview_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/treectrl.h>
#include <wx/sizer.h>

#include "../simulator/network.h"

class NetworkView: public wxPanel
{
public:
  NetworkView(wxWindow *parent, wxWindowID id=wxID_ANY);
  virtual ~NetworkView(){};
  
  void loadNetwork(Network *network);

  wxTreeCtrl *m_treectrl;
private:
  Network *_network;
  //wxDECLARE_EVENT_TABLE();
};

#endif /*networkview.h*/
