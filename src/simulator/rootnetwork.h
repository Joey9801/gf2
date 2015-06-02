#ifndef ROOTNETWORK_H_
#define ROOTNETWORK_H_

#include <string>
#include <vector>

#include <plog/Log.h>

#include "network.h"
#include "../structures/nodetree.h"

// Special type of network which can operate independantly from anything else
// Should only be used as the Root of the whole network
class RootNetwork : public Network
{
  public:
    RootNetwork(Network * net);
    ~RootNetwork();

    using Network::step;
    void step(void);

    void setInput(unsigned int inputId, bool value);
    void setInput(std::string inputName, bool value);
    bool getInput(unsigned int inputId);
    bool getInput(std::string inputName);

    bool getOutput(unsigned int outputId);
    bool getOutput(std::string outputName);
    
    NodeTree * getNodeTree(void);

    unsigned int addMonitorPoint(std::vector<std::string>& signature);
    void removeMonitorPoint(std::vector<std::string>& signature);

  private:
    std::vector<bool> _inputVals;
};

#endif
