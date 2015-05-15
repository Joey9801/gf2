#include "signalgenerator.h"

SignalGenerator::SignalGenerator() :
  BaseComponent("Signal Generator", 0, 1)
{
  _pinOutMap["out"] = 0;
  _data = std::vector<bool>(1, false);
  _period = 1;

  _count = 0;
  _it = _data.begin();
}

SignalGenerator::~SignalGenerator() {}

void SignalGenerator::step(std::vector<bool>& a, std::vector<bool>& b) {
  b[_outputs[0]] = *_it;

  _count++;
  if(_count >= _period) {
    _count = 0;
    _it++;
    if(_it == _data.end())
      _it = _data.begin();
  }

  return;
}

void SignalGenerator::config(std::string key, std::string value) {
  if(key=="data") {

    _data.clear();

    for(std::string::iterator it = value.begin(); it != value.end(); it++) {
      if( (*it=='t') or (*it=='1') )
        _data.push_back(true);
      else
        _data.push_back(false);
    }

  }
  else if (key=="period") {
      std::istringstream ( value ) >> _period;
  }
  return;
}