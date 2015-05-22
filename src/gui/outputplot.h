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

#include "../simulator/monitor.h"

class MyGLCanvas: public wxGLCanvas
{
  public:
    MyGLCanvas(wxWindow *parent, wxWindowID id); // constructor
    std::map<wxString, unsigned int> _monitortraces;
    void Render(); // function to draw canvas contents
    void setMonitor(Monitor * m);

  private:
    wxGLContext *context;              // OpenGL rendering context
    bool init;                         // has the OpenGL context been initialised?
    void InitGL();                     // function to initialise OpenGL context
    void OnSize(wxSizeEvent& event);   // event handler for when canvas is resized
    void OnPaint(wxPaintEvent& event); // event handler for when canvas is exposed
    void OnMousewheel(wxMouseEvent& event); // event handler for when mousewheel zooming

    void drawAxis();
    void drawPlot(unsigned int num, const wxString& label, const std::vector<bool>& data);

    float xzero, yzero;
    float rowheight, bitwidth;

    Monitor * _monitor;
};

class OutputPlot: public wxScrolledWindow
{
  public:
    OutputPlot(wxWindow *parent, wxWindowID id=wxID_ANY);
    virtual ~OutputPlot(){};

    void AddPlotTrace(std::string label, unsigned int pointId);
    void setMonitor(Monitor * m);

    void refresh(void);

  private:
    MyGLCanvas *_plotcanvas;
    Monitor * _monitor;
};

#endif /*outputplot.h*/
