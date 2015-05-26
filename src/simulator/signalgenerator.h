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

    virtual void configure(std::string key, std::string value);

    BaseComponent * clone(void);

  protected:
    unsigned int _period;
    std::vector<bool> _data;

    unsigned int _count;
    std::vector<bool>::iterator _it;
};

class Clock : public SignalGenerator
{
  public:
    Clock();
    virtual void configure(std::string key, std::string value);
};

#endif
