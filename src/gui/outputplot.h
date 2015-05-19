#ifndef outputplot_h
#define outputplot_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include <wx/textctrl.h>
#include <vector>
#include <string>

#define wxUSE_UNICODE 1

using namespace std;

class OutputPlot: public wxPanel
{
public:
  OutputPlot(wxWindow *parent, wxWindowID id=wxID_ANY);
  virtual ~OutputPlot(){};
  
  void plotData(string label, vector<bool> &data);
private:
  wxTextCtrl *_plotfield;
};

#endif /*outputplot.h*/
