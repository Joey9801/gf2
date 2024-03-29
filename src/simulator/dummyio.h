#ifndef DUMMYIO_H_
#define DUMMYIO_H_

#include <string>
#include <vector>
#include <map>

#include "basecomponent.h"

class Network;
class RootNetwork;

class DummyIO : public BaseComponent
{
  friend class Network;
  friend class RootNetwork;
  public:
    DummyIO();
    ~DummyIO();

    void step(std::vector<bool>& a, std::vector<bool>& b);
    void loadInputs(std::vector<bool>& source, std::vector<bool>& sink, std::vector<unsigned int> indicies);
    void loadOutputs(std::vector<bool>& source, std::vector<bool>& sink, std::vector<unsigned int> indicies);

    unsigned int addInput();
    unsigned int addInput(std::string name);
    unsigned int addOutput();
    unsigned int addOutput(std::string name);
};

#endif
