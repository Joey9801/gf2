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
    
    //Network functions
    virtual unsigned int addComponent(std::string) {return 0;}
    virtual unsigned int addInput(void) {return 0;}
    virtual unsigned int addOutput(void) {return 0;}
    virtual void connect(unsigned int, unsigned int, unsigned int, unsigned int) {}
    virtual void connectInput(unsigned int, unsigned int, unsigned int) {}
    virtual void connectOutput(unsigned int, unsigned int, unsigned int) {}

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
    virtual void setInputs(std::vector<unsigned int> nodes);
    virtual void setInput(std::string name, unsigned int node);
    virtual void setInput(unsigned int inputId, unsigned int node);

  protected:
    //virtual bool _compare(bool a, bool b) =0;
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
typedef std::map<std::string, BaseComponent*(*)(void)> component_map;

#endif
