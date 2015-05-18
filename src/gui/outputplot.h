#ifndef outputplot_h
#define outputplot_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif


class OutputPlot: public wxPanel
{
public:
  OutputPlot(wxWindow *parent, wxWindowID id=wxID_ANY);
  virtual ~OutputPlot(){};
  
private:
};

#endif /*outputplot.h*/
