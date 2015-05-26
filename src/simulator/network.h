#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <vector>
#include <sstream>

#include <plog/Log.h>

#include "basecomponent.h"
#include "dummyio.h"
#include "componentconstructors.h"
#include "monitor.h"

extern constructor_map componentConstructor;

class Network : public BaseComponent
{
  public:
    Network();
    ~Network();

    void step(std::vector<bool>& a, std::vector<bool>& b);

    unsigned int addComponent(std::string type);
    unsigned int addComponent(std::string type, std::string name);
    unsigned int addComponent(BaseComponent * c);
    unsigned int addComponent(BaseComponent * c, std::string name);

    void configureComponent(std::string name, std::string key, std::string value);
    void configureComponent(unsigned int componentId, std::string key, std::string value);

    void renameComponent(std::string oldName, std::string newName);
    void renameComponent(unsigned int id, std::string newName);
    unsigned int findComponent(unsigned int componentId);
    unsigned int findComponent(std::string componentName);
    unsigned int constNode(unsigned int constVal);
    unsigned int constNode(std::string constVal);

    bool isInputVector(unsigned int inputId);
    bool isInputVector(std::string inputName);
    bool isOutputVector(unsigned int outputId);
    bool isOutputVector(std::string outputName);

    unsigned int addInput(void);
    unsigned int addInput(std::string);
    unsigned int addVectorInput(std::string);
    unsigned int addOutput(void);
    unsigned int addOutput(std::string);
    unsigned int addVectorOutput(std::string);

    // Connecting two components
    // Annoying that templates must be defined in the header, but whatever
    // The input 'pinIn' of componentIn is connected to the output 'pinOut' of componentOut
    // Each argument may be a string or an unsigned integer. Strings for component/pin names,
    // unsigned ints for component/pin id's
    template<typename t1, typename t2, typename t3, typename t4>
      void connect(t1 componentOut, t2 pinOut, t3 componentIn, t4 pinIn) {
        unsigned int idOut = findComponent(componentOut);
        unsigned int idIn = findComponent(componentIn);

        if( idOut == (compl 0u) ) {
          //Magic number for const
          unsigned int node = constNode(pinOut);
          _components[idIn]->connectInput(pinIn, node);
        }
        else if(_components[idOut]->isOutputVector(pinOut)
            and _components[idIn]->isInputVector(pinIn) ) {
          std::vector<unsigned int> nodes = _components[idOut]->getOutputVectorNodes(pinOut);
          _components[idIn]->connectVectorInput(pinIn, nodes);
        }
        else {
          unsigned int node = _components[idOut]->getOutputNode(pinOut);
          _components[idIn]->connectInput(pinIn, node);
        }

        numConnections++;
        return;
      }

    unsigned int countComponents(void);

    void setMonitor(Monitor * m);
    unsigned int addMonitorPoint(std::vector<std::string>& signature);
    unsigned int addMonitorPoint(std::vector<std::string>& signature, unsigned int depth);

    void removeMonitorPoint(std::vector<std::string>& signature);
    void removeMonitorPoint(std::vector<std::string>& signature, unsigned int depth);

    NodeTree * getNodeTree(void);

    BaseComponent * clone(void);

    unsigned int numConnections;

  protected:
    std::map<std::string, unsigned int> _componentNames;
    std::vector<BaseComponent*> _components;

    DummyIO * _inputDummy;
    DummyIO * _outputDummy;

    std::vector<bool> _nodesA;
    std::vector<bool> _nodesB;

    Monitor * _monitor;

    std::map<unsigned int, unsigned int> _monitorPoints;

    unsigned int _time;

};

// Special type of network which can operate independantly from anything else
// Should only be used as the Root of the whole network
class RootNetwork : public Network
{
  public:
    RootNetwork();
    ~RootNetwork();

    using Network::step;
    void step(void);

    unsigned int addInput(void);
    unsigned int addInput(std::string);
    unsigned int addOutput(void);
    unsigned int addOutput(std::string);

    void setInput(unsigned int inputId, bool value);
    void setInput(std::string inputName, bool value);
    bool getOutput(unsigned int outputId);
    bool getOutput(std::string outputName);
};

#endif
