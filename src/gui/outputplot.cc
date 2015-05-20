#include "outputplot.h"
#include <iostream>

OutputPlot::OutputPlot(wxWindow *parent, wxWindowID id)
  :   wxPanel(parent, id)
{
  //make some fake command line args for glutinit
  char a  =  ' '; char *b = &a; char **tmp1 = &b;
  int tmp2 = 0; glutInit(&tmp2, tmp1);

  _plotcanvas = new MyGLCanvas(this, -1);

  wxBoxSizer *nvsizer = new wxBoxSizer(wxVERTICAL);
  nvsizer->Add(_plotcanvas, 1,wxEXPAND,0);
  SetSizer(nvsizer);
}

int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id) :
  wxGLCanvas(parent, id, wxglcanvas_attrib_list)
  // Constructor - initialises private variables
{
  context = new wxGLContext(this);
  init = false;

  Bind(wxEVT_SIZE, &MyGLCanvas::OnSize, this);
  Bind(wxEVT_PAINT, &MyGLCanvas::OnPaint, this);

  
  bool dataArray[] = {true, false, true, false, true, true, true, true, false, false, true};
  vector<bool> data1 (dataArray, dataArray + sizeof(dataArray) / sizeof(bool));
  bool dataArray2[] = {false, false, true, false, false, true, false, true, false, false, true};
  vector<bool> data2 (dataArray2, dataArray2 + sizeof(dataArray2) / sizeof(bool));
  _monitortraces["PLOT1"] = data1;
  _monitortraces["PLOT2"] = data2;
}

void MyGLCanvas::Render()
{
  float y;
  unsigned int i;

  SetCurrent(*context);
  if (!init) {
    InitGL();
    init = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  float rowheight = GetSize().y / _monitortraces.size();
  float plotheight = 0.8*rowheight, bitwidth = 30.0;
  float yzero  = 10.0, xzero = 100.0;
  typedef map<wxString, vector<bool>>::iterator it_type;
  for(it_type it=_monitortraces.begin(); it!=_monitortraces.end(); it++) {
    glColor3f(0.0, 0.0, 1.0);
    glRasterPos2f(10, yzero);
    wxString plotlabel = it->first.Left(10);
    for (i = 0; i < plotlabel.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, plotlabel[i]);

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i<it->second.size(); i++) {
      if (it->second[i]) y = plotheight;
      else y = 0.0;
      glVertex2f(xzero + i*bitwidth, yzero+y); 
      glVertex2f(xzero + (i+1)*bitwidth, yzero+y);
    }
    yzero += rowheight;
    glEnd();
  } 

  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
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

void MyGLCanvas::OnPaint(wxPaintEvent& event)
  // Event handler for when the canvas is exposed
{
  wxPaintDC dc(this); // required for correct refreshing under MS windows
  Render();
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
  // Event handler for when the canvas is resized
{
  init = false;; // this will force the viewport and projection matrices to be reconfigured on the next paint
}
