#ifndef SIGNALGENERATOR_H_
#define SIGNALGENERATOR_H_

#include <string>
#include <vector>
#include <sstream>

#include "basecomponent.h"

class SignalGenerator : public BaseComponent
{
  public:
    SignalGenerator();
    ~SignalGenerator();

    void step(std::vector<bool>& a, std::vector<bool>& b);

    void configure(std::string key, std::string value);

  private:
    unsigned int _period;
    std::vector<bool> _data;

    unsigned int _count;
    std::vector<bool>::iterator _it;
};

#endif
