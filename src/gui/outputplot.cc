#include "outputplot.h"

int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

OutputPlot::OutputPlot(wxWindow *parent, wxWindowID id)
  :   wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL)
{
  //make some fake command line args for glutinit
  char a  =  ' '; char *b = &a; char **tmp1 = &b;
  int tmp2 = 0; glutInit(&tmp2, tmp1);

  _plotcanvas = new MyGLCanvas(this, -1);

  wxBoxSizer *nvsizer = new wxBoxSizer(wxVERTICAL);
  nvsizer->Add(_plotcanvas, 1,wxEXPAND,0);
  SetSizer(nvsizer);
  SetScrollRate(10, 10);
  SetAutoLayout(true);

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

void MyGLCanvas::setMonitor(Monitor * m) {
  _monitor = m;
  return;
}

MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id) :
  wxGLCanvas(parent, id, wxglcanvas_attrib_list)
  // Constructor - initialises private variables
{
  context = new wxGLContext(this);
  init = false;

  Bind(wxEVT_SIZE, &MyGLCanvas::OnSize, this);
  Bind(wxEVT_PAINT, &MyGLCanvas::OnPaint, this);
  Bind(wxEVT_MOUSEWHEEL, &MyGLCanvas::OnMousewheel, this);

  bitwidth = 30.0;
  xzero = 100.0;
  yzero  = 20.0;
}

void MyGLCanvas::Render()
{
  SetCurrent(*context);
  if (!init) {
    InitGL();
    init = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  rowheight = 50;
  SetMinSize(wxSize(_monitor->maxLength*bitwidth, -1));

  drawAxis();

  std::vector<unsigned int> ids = _monitor->getPoints();
  for(unsigned int i=0; i<ids.size(); i++)
    drawPlot(i, _monitor->getNickname(ids[i]), _monitor->getLog(ids[i]));

  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
}

void MyGLCanvas::drawAxis(void) {
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

void MyGLCanvas::drawPlot(
    unsigned int num,
    const wxString& label,
    const std::vector<std::pair<unsigned int, bool> >& data)
{
  float base = yzero + (num * rowheight);

  //write labels, wrap line if longer than 9 chars
  //if there is not enough vertical space, label is truncated
  glColor3f(0.0, 0.0, 1.0);
  for (unsigned int i = 0.0; label.Len()>9*i and rowheight*0.6>i*17.0; i++) {
    glRasterPos2f(10, base + rowheight*0.6-17.0*i);//label pos
    wxString plotlabel = label.Mid(i*9, 9);//truncate label to fit
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

void MyGLCanvas::InitGL()
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
void MyGLCanvas::OnPaint(wxPaintEvent& event) {
  // required for correct refreshing under MS windows
  wxPaintDC dc(this);
  Render();
}

// Event handler for when the canvas is resized
void MyGLCanvas::OnSize(wxSizeEvent& event) {
  // this will force the viewport and projection matrices to be reconfigured on the next paint
  init = false;
}

void MyGLCanvas::OnMousewheel(wxMouseEvent& event) {
  if (event.GetWheelRotation()>0.0) {
    bitwidth += 5;
  }
  else if (event.GetWheelRotation()<0.0) {
    bitwidth -= 5.0;
  }
  if (bitwidth<5.0) {
    bitwidth = 5.0;
  }
  init = false;
  Render();
}
