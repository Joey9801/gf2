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

    void connectOutputs(unsigned int index);
    void connectInputs(std::vector<unsigned int> nodes);
    virtual void connectInput(std::string name, unsigned int node);
    virtual void connectInput(unsigned int index, unsigned int node);

  protected:
    //virtual bool _compare(bool a, bool b) =0;
    std::string _name;

    pin_map pinMap;

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
