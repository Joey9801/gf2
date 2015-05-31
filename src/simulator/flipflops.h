#ifndef FLIPFLOPS_H_
#define FLIPFLOPS_H_

#include "basecomponent.h"
#include <vector>
#include <string>

class DType : public BaseComponent
{
  public:
    DType();
    ~DType();
    void step(std::vector<bool>& a, std::vector<bool>& b);

    virtual void Reset();
  private:
    // Used to detect rising/falling edges
    bool _lastIn;

    // Used to maintain output between steps, in case its
    // output node was written to by something else
    bool _lastOut;
};

class JK : public BaseComponent
{
  public:
    JK();
    ~JK();
    void step(std::vector<bool>& a, std::vector<bool>& b);

    virtual void Reset();
  private:
    bool _lastIn, _lastOut;
};

#endif
