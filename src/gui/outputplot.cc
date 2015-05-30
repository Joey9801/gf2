#include "outputplot.h"

int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

OutputPlot::OutputPlot(wxWindow *parent, wxWindowID id)
  :   wxPanel(parent, id)
{
  _simulationControl = new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
  wxImage::AddHandler( new wxPNGHandler );
  
  wxBitmap runImg(wxT("resources/control_play.png"), wxBITMAP_TYPE_PNG);
  wxBitmapButton *runButton = new wxBitmapButton(_simulationControl, ID_RunSim, runImg);
  runButton->SetToolTip(_("Run Simulation"));
  _simulationControl->AddControl(runButton);

  wxBitmap pauseImg(wxT("resources/control_pause.png"), wxBITMAP_TYPE_PNG);
  wxBitmapButton *pauseButton = new wxBitmapButton(_simulationControl, ID_PauseSim, pauseImg);
  pauseButton->SetToolTip(_("Pause Simulation"));
  _simulationControl->AddControl(pauseButton);

  wxBitmap stopImg(wxT("resources/control_stop.png"), wxBITMAP_TYPE_PNG);
  wxBitmapButton *stopButton = new wxBitmapButton(_simulationControl, ID_StopSim, stopImg);
  stopButton->SetToolTip(_("Stop Simulation"));
  _simulationControl->AddControl(stopButton);

  _simulationControl->AddControl(new wxComboBox(_simulationControl, ID_SimulationSteps));

  _simulationControl->Realize();

  //make some fake command line args for glutinit
  char a  =  ' '; char *b = &a; char **tmp1 = &b;
  int tmp2 = 0; glutInit(&tmp2, tmp1);

  wxScrolledWindow *canvasscroller = new wxScrolledWindow(this, -1);
  _plotcanvas = new PlotCanvas(canvasscroller, -1);
  wxBoxSizer *scrolledsizer = new wxBoxSizer(wxVERTICAL);
  scrolledsizer->Add(_plotcanvas, 1,wxEXPAND,0);
  canvasscroller->SetSizer(scrolledsizer);
  canvasscroller->SetScrollRate(10, 10);
  canvasscroller->SetAutoLayout(true);

  wxBoxSizer *opsizer = new wxBoxSizer(wxVERTICAL);
  opsizer->Add(_simulationControl, 0,wxEXPAND,0);
  opsizer->Add(canvasscroller, 1,wxEXPAND,0);
  SetSizer(opsizer);
}

void OutputPlot::setMonitor(Monitor * m) {
  _monitor = m;
  _plotcanvas -> setMonitor(m);
  return;
}

void OutputPlot::refresh(void) {
  _plotcanvas->Render();
  return;
}

void PlotCanvas::setMonitor(Monitor * m) {
  _monitor = m;
  return;
}

PlotCanvas::PlotCanvas(wxWindow *parent, wxWindowID id) :
  wxGLCanvas(parent, id, wxglcanvas_attrib_list)
  // Constructor - initialises private variables
{
  context = new wxGLContext(this);
  init = false;

  Bind(wxEVT_SIZE, &PlotCanvas::OnSize, this);
  Bind(wxEVT_PAINT, &PlotCanvas::OnPaint, this);
  Bind(wxEVT_MOUSEWHEEL, &PlotCanvas::OnMousewheel, this);

  bitwidth = 31.0;
  xzero = 200.0;
  yzero  = 20.0;
}

void PlotCanvas::Render()
{
  LOG_DEBUG;
  std::vector<unsigned int> ids = _monitor->getPoints();

  SetMinSize(wxSize(_monitor->maxLength*bitwidth, yzero + rowheight * ids.size()));
  SendSizeEventToParent();
  //FitInside();

  SetCurrent(*context);
  if (!init) {
    InitGL();
    init = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  rowheight = 50;

  drawAxis();

  for(unsigned int i=0; i<ids.size(); i++)
    drawPlot(i, _monitor->getNickname(ids[i]), _monitor->getLog(ids[i]));

  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
}

void PlotCanvas::drawAxis(void) {
  //draw axis line
  glColor3f(0.0, 0.0, 0.0);//axis colour
    glBegin(GL_LINE_STRIP);
    glVertex2f(xzero, yzero*0.5);
    glVertex2f(GetSize().x-5.0, yzero*0.5);
  glEnd();
  //draw arrowhead
  glBegin(GL_TRIANGLES);
    glVertex2f(GetSize().x-15.0, yzero*0.2);
    glVertex2f(GetSize().x-15.0, yzero*0.8);
    glVertex2f(GetSize().x-5.0, yzero*0.5);
  glEnd();
  //draw tickmarks
  for(float x = xzero; x<GetSize().x-15.0; x+=bitwidth){
    glBegin(GL_LINE_STRIP);
      glVertex2f(x, yzero*0.7);
      glVertex2f(x, yzero*0.3);
    glEnd();
  }
  return;
}

void PlotCanvas::drawPlot(
    unsigned int num,
    const wxString& label,
    const std::vector<std::pair<unsigned int, bool> >& data)
{
  float base = yzero + (num * rowheight);

  //write labels, wrap line if longer than 18 chars
  //if there is not enough vertical space, label is truncated
  glColor3f(0.0, 0.0, 1.0);
  for (unsigned int i = 0.0; label.Len()>18*i and rowheight*0.6>i*17.0; i++) {
    glRasterPos2f(10, base + rowheight*0.6-17.0*i);//label pos
    wxString plotlabel = label.Mid(i*18, 18);//truncate label to fit
    //write label
    for (unsigned int j = 0; j < plotlabel.Len(); j++)
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, plotlabel[j]);
  }

  //draw actual plot traces
  glColor3f(0.0, 1.0, 0.0);//plot colour
  glBegin(GL_LINE_STRIP);
  for (unsigned int i = 0; i<data.size(); i++) {

    float y = data[i].second ? (rowheight*0.8) : 0;

    glVertex2f(xzero + data[i].first * bitwidth, base + y);
    glVertex2f(xzero + (data[i].first+1) * bitwidth, base + y);
  }
  glEnd();
  return;
}

void PlotCanvas::InitGL()
  // Function to initialise the GL context
{
  int w, h;

  GetClientSize(&w, &h);
  SetCurrent(*context);
  glDrawBuffer(GL_BACK);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Event handler for when the canvas is exposed
void PlotCanvas::OnPaint(wxPaintEvent& event) {
  LOG_DEBUG;
  // required for correct refreshing under MS windows
  wxPaintDC dc(this);
  Render();
}

// Event handler for when the canvas is resized
void PlotCanvas::OnSize(wxSizeEvent& event) {
  LOG_DEBUG;
  // this will force the viewport and projection matrices to be reconfigured on the next paint
  init = false;
}

void PlotCanvas::OnMousewheel(wxMouseEvent& event) {
  LOG_DEBUG;
  if (event.GetWheelRotation()>0.0) {
    bitwidth += 5;
  }
  else if (event.GetWheelRotation()<0.0) {
    bitwidth -= 5.0;
  }
  if (bitwidth<1.0) {
    bitwidth = 1.0;
  }
  init = false;
  Render();
}
