#include "outputplot.h"
#include <iostream>

OutputPlot::OutputPlot(wxWindow *parent, wxWindowID id)
  :   wxPanel(parent, id)
{
  _plotfield = new wxTextCtrl(this, -1, "Plotfield:\n", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  _plotfield->SetEditable(false);

  wxBoxSizer *nvsizer = new wxBoxSizer(wxVERTICAL);
  nvsizer->Add(_plotfield, 1,wxEXPAND,0);
  SetSizer(nvsizer);
}

void OutputPlot::plotData(string label, vector<bool> &data)
{
  vector<wxString> plot(5, "");
  for(unsigned int i = 0; i < data.size(); i++) {
    if (i == data.size()-1){
      if (data[i]==true){
        plot[0].append("\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80");
        plot[1].append("   ");
        plot[2].append("   ");
      }else {
        plot[0].append("   ");
        plot[1].append("   ");
        plot[2].append("\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80");
      }
    }else {
      if (data[i]==true and data[i+1]==true){ 
        plot[0].append("\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80");
        plot[1].append("   ");
        plot[2].append("   ");
      }else if (data[i]==true and data[i+1]==false){
        plot[0].append("\xe2\x94\x80\xe2\x94\x80");
        plot[0].append("\xe2\x94\x90");
        plot[1].append("  ");
        plot[1].append("\xe2\x94\x82");
        plot[2].append("  ");
        plot[2].append("\xe2\x94\x94");
      }else if (data[i]==false and data[i+1]==true){
        plot[0].append("  ");
        plot[0].append("\xe2\x94\x8c");
        plot[1].append("  ");
        plot[1].append("\xe2\x94\x82");
        plot[2].append("\xe2\x94\x80\xe2\x94\x80");
        plot[2].append("\xe2\x94\x98");
      }else if (data[i]==false and data[i+1]==false){
        plot[0].append("   ");
        plot[1].append("   ");
        plot[2].append("\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80");
      }
    }
  }

  for(unsigned int i = 0; i < plot.size(); i++) {
    cout << plot[i] << endl;
    _plotfield->AppendText(plot[i]);
    _plotfield->AppendText("\n");
  }
}
