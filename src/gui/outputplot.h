#ifndef outputplot_h
#define outputplot_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include <GL/glut.h>
#include <wx/glcanvas.h>
#include <wx/scrolbar.h>
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/numdlg.h>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>

#include <plog/Log.h>

#include "../simulator/monitor.h"
#include "../simulator/rootnetwork.h"

class PlotCanvas;

class OutputPlot: public wxPanel
{
  public:
    OutputPlot(wxWindow *parent, wxWindowID id=wxID_ANY);
    virtual ~OutputPlot(){};

    void AddPlotTrace(std::string label, unsigned int pointId);
    void setMonitor(Monitor * m);
    void setNetwork(RootNetwork * n);
    void EnableToolbar(bool enabled);

    void refresh(void);

    enum {
      ID_RunSim = 1,
      ID_PauseSim,
      ID_StopSim,
      ID_SkipSim
    };

    void OnMousewheel(wxMouseEvent& event); // event handler for when mousewheel zooming
    void OnScroll(wxScrollEvent& event);
  private:
    Monitor * _monitor;
    RootNetwork * _network;

    PlotCanvas *_plotcanvas;
    wxScrollBar *_canvasscrollbar;
    wxScrolledWindow *_canvasscroller;
    wxToolBar *_simulationControl;

    wxTimer *_liveSimulationTimer;

    void OnRunButton(wxCommandEvent& event);
    void OnPauseButton(wxCommandEvent& event);
    void OnStopButton(wxCommandEvent& event);
    void OnSkipButton(wxCommandEvent& event);
    void OnLiveSimulationStep(wxTimerEvent& event);

    float bitwidth;
};

bool isIndexGreater(std::pair<unsigned int, bool> target, std::pair<unsigned int, bool> datapoint);

class PlotCanvas: public wxGLCanvas
{
  public:
    PlotCanvas(wxWindow *parent, wxWindowID id); // constructor
    void Render(unsigned int xpos, float bitwidth); // function to draw canvas contents
    void setMonitor(Monitor * m);

  private:
    wxGLContext *context;              // OpenGL rendering context
    bool init;                         // has the OpenGL context been initialised?
    void InitGL();                     // function to initialise OpenGL context
    void OnSize(wxSizeEvent& event);   // event handler for when canvas is resized
    void OnPaint(wxPaintEvent& event); // event handler for when canvas is exposed

    void drawAxis(unsigned int xpos, float bitwidth);
    void drawPlot(unsigned int num, const wxString& label,
        const std::vector<std::pair<unsigned int, bool> >& data,
        unsigned int xpos, float bitwidth);
    unsigned int _currentxpos;
    float _currentbitwidth;

    float xzero, yzero;
    float rowheight, bitwidth;

    Monitor * _monitor;
};

#endif /*outputplot.h*/
