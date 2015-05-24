#include "signalgenerator.h"

SignalGenerator::SignalGenerator() :
  BaseComponent("siggen", 0, 1)
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

void SignalGenerator::configure(std::string key, std::string value) {
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

BaseComponent * SignalGenerator::clone(void) {
  SignalGenerator * c = new SignalGenerator();

  c->_inputs = _inputs;
  c->_outputs = _outputs;
  c->_period = _period;
  c->_data = _data;
  c->_it = c->_data.begin();

  return (BaseComponent*)c;
}
