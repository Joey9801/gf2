#include "outputplot.h"

//wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
//wxEND_EVENT_TABLE()

OutputPlot::OutputPlot(wxWindow *parent, wxWindowID id)
  :   wxPanel(parent, id)
{
  /*
  // Create a mpFXYVector layer
  mpFXYVector* vectorLayer = new mpFXYVector(_("Vector"));
  // Create two vectors for x,y and fill them with data
  std::vector<double> vectorx, vectory;
  double xcoord;
  for (unsigned int p = 0; p < 100; p++) {
  xcoord = ((double)p-50.0)*5.0;
  vectorx.push_back(xcoord);
  vectory.push_back(0.0001*pow(xcoord, 3));
  }
  vectorLayer->SetData(vectorx, vectory);
  vectorLayer->SetContinuity(true);
  wxPen vectorpen(*wxBLUE, 2, wxSOLID);
  vectorLayer->SetPen(vectorpen);
  vectorLayer->SetDrawOutsideMargins(false);

  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(100,100), wxSUNKEN_BORDER );
  mpScaleX* xaxis = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
  mpScaleY* yaxis = new mpScaleY(wxT("Y"), mpALIGN_LEFT, true);
  xaxis->SetFont(graphFont);
  yaxis->SetFont(graphFont);
  xaxis->SetDrawOutsideMargins(false);
  yaxis->SetDrawOutsideMargins(false);
  // Fake axes formatting to test arbitrary format string
  // xaxis->SetLabelFormat(wxT("%.2f €"));
  // yaxis->SetLabelFormat(wxT("%p"));
  m_plot->SetMargins(30, 30, 50, 100);
  //m_plot->SetMargins(50, 50, 200, 150);
  m_plot->AddLayer(xaxis );
  m_plot->AddLayer(yaxis );
  m_plot->AddLayer(vectorLayer );
  */
}
