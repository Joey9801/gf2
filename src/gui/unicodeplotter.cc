#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> digitalplot(const vector<bool> &data)
{
  vector<string> plot(5, "");
  for(int i = 0; i < data.size()-1; i++) {
    if(data[i]==true and data[i+1]==true){ 
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
  return plot;
}

int main()
{
  bool dataArray[] = {true, false, true, false, true, true, true, true, false, false, true};
  vector<bool> data (dataArray, dataArray + sizeof(dataArray) / sizeof(bool));
  vector<string> plot = digitalplot(data);
  for(int i = 0; i < plot.size(); i++) {
        cout << plot[i] << endl; 
  }
}

