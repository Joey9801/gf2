#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <vector>

#include "basecomponent.h"
#include "boolgates.h"

class Network : public BaseComponent
{
  public:
    Network();
    ~Network();

    void step(std::vector<bool>& a, std::vector<bool>& b);

    unsigned int addComponent(std::string type);
    unsigned int addInput(void);
    unsigned int addOutput(void);

    //Connecting two components
    void connect(unsigned int idOut, unsigned int pinOut, unsigned int idIn, unsigned int pinIn);

    //Connecting a component to an input
    void connectInput(unsigned int inputId, unsigned int idIn, unsigned int pinIn);
    void connectOutput(unsigned int outputId, unsigned int idOut, unsigned int pinOut);

    unsigned int countComponents(void);
  private:
    component_map _componentMap;

    std::vector<unsigned int> _inputMap;      //Distinct from _inputs. Maps _input[i] to internal _node[j]
    std::vector<unsigned int> _outputMap;      //Distinct from _outputs. Maps _output[i] to internal _node[j]
    std::vector<BaseComponent*> _components;
    std::vector<bool> _nodesA;
    std::vector<bool> _nodesB;
};

#endif
