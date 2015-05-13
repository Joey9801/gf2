#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <vector>
#include <string>
#include <map>

typedef std::map<std::string, unsigned int> pin_map;

class BaseComponent {
  public:
    BaseComponent(std::string name);
    BaseComponent(std::string name, unsigned int numInputs, unsigned int numOutputs);
    virtual ~BaseComponent();
    virtual void step(std::vector<bool>& a, std::vector<bool>& b) =0;
    
    std::string getName();

    void renameInput(std::string oldName, std::string newName);
    void renameInput(unsigned int inputId, std::string newName);
    void renameOutput(std::string oldName, std::string newName);
    void renameOutput(unsigned int outputId, std::string newName);

    unsigned int getOutputNode(unsigned int pinOut);
    unsigned int getOutputNode(std::string name);
    unsigned int numOutputs(void);

    unsigned int getInputNode(unsigned int pinIn);
    unsigned int getInputNode(std::string name);
    unsigned int numInputs(void);

    virtual void setOutput(unsigned int outputId, unsigned int node);
    virtual void setOutput(std::string name, unsigned int node);
    virtual void setInput(unsigned int inputId, unsigned int node);
    virtual void setInput(std::string name, unsigned int node);

  protected:
    std::string _name;

    pin_map _pinInMap;
    pin_map _pinOutMap;

    std::vector<unsigned int> _inputs;
    std::vector<unsigned int> _outputs;
};

//In lieu of proper reflection, the following will have to do
//Maps strings to a (sort of) factory for components
//  Usage:
//    component_map componentMap;
//    componentMap["and"] = &createComponent<AndGate>;
//
//    BaseComponent * gate = componentMap["and"]()
//    //Set up in/output references
//    gate->step(a, b);
//    etc..
template<typename T> BaseComponent * createComponent() {
  return new T();
}
typedef std::map<std::string, BaseComponent*(*)(void)> constructor_map;

#endif
