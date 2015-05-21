#ifndef outputplot_h
#define outputplot_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include <GL/glut.h>
#include <wx/glcanvas.h>
#include <wx/scrolwin.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

class MyGLCanvas: public wxGLCanvas
{
public:
  MyGLCanvas(wxWindow *parent, wxWindowID id); // constructor
  map<wxString, vector<bool>> _monitortraces;
  void Render(); // function to draw canvas contents
private:
  wxGLContext *context;              // OpenGL rendering context
  bool init;                         // has the OpenGL context been initialised?
  void InitGL();                     // function to initialise OpenGL context
  void OnSize(wxSizeEvent& event);   // event handler for when canvas is resized
  void OnPaint(wxPaintEvent& event); // event handler for when canvas is exposed

  float rowheight, bitwidth;
};

class OutputPlot: public wxScrolledWindow
{
public:
  OutputPlot(wxWindow *parent, wxWindowID id=wxID_ANY);
  virtual ~OutputPlot(){};
  
  void AddPlotTrace(string label, vector<bool> &data);
private:
  MyGLCanvas *_plotcanvas;
};

#endif /*outputplot.h*/
