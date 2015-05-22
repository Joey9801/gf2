#ifndef networkview_h
#define networkview_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/treectrl.h>
#include <wx/sizer.h>

#include <plog/Log.h>

#include "../structures/nodetree.h"

class NetworkView: public wxPanel
{
  public:
    NetworkView(wxWindow *parent, wxWindowID id=wxID_ANY);
    virtual ~NetworkView(){};

    void loadNetwork(NodeTreeBase *network);

    wxTreeCtrl *_treectrl;
  private:
    void displayNetwork();
    void recursive_addNode(wxTreeItemId parentid, NodeTreeBase *node);

    NodeTreeBase *_network;
};

#endif /*networkview.h*/
