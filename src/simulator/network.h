#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <vector>

#include "basecomponent.h"
#include "boolgates.h"
#include "flipflops.h"

//Dummy IO component
class DummyIO : public BaseComponent
{
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

class Network : public BaseComponent
{
  public:
    Network();
    ~Network();

    void step(std::vector<bool>& a, std::vector<bool>& b);

    unsigned int addComponent(std::string type);
    unsigned int addComponent(std::string type, std::string name);

    void renameComponent(std::string oldName, std::string newName);
    void renameComponent(unsigned int id, std::string newName);
    unsigned int findComponent(unsigned int componentId);
    unsigned int findComponent(std::string componentName);

    unsigned int addInput(void);
    unsigned int addInput(std::string);
    unsigned int addOutput(void);
    unsigned int addOutput(std::string);

    // Connecting two components
    // Annoying that templates must be defined in the header, but whatever
    // The input 'pinIn' of componentIn is connected to the output 'pinOut' of componentOut
    // Each argument may be a string or an unsigned integer. Strings for component/pin names,
    // unsigned ints for component/pin id's
    template<typename t1, typename t2, typename t3, typename t4>
      void connect(t1 componentOut, t2 pinOut, t3 componentIn, t4 pinIn) {
        unsigned int idOut = findComponent(componentOut);
        unsigned int idIn = findComponent(componentIn);

        unsigned int node = _components[idOut]->getOutputNode(pinOut);
        _components[idIn]->setInput(pinIn, node);
        return;
      }

    unsigned int countComponents(void);

  private:
    constructor_map _componentConstructor;

    std::map<std::string, unsigned int> _componentNames;
    std::vector<BaseComponent*> _components;

    DummyIO * _inputDummy;
    DummyIO * _outputDummy;

    std::vector<bool> _nodesA;
    std::vector<bool> _nodesB;
};

#endif
