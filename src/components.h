#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <vector>
#include <string>
#include <map>

class BaseComponent {
  public:
    BaseComponent(std::string name, std::vector<unsigned int> nodes);
    virtual ~BaseComponent();
    virtual void step(std::vector<bool>& in, std::vector<bool>& out) =0;

    std::string getName();
    std::vector<unsigned int> getNodes();

  protected:
    std::string _name;
    std::vector<unsigned int> _nodes;
};

class AndGate : public BaseComponent {
  public:
    AndGate(std::vector<unsigned int> nodes)
      : BaseComponent("And", nodes) {}

    void step(std::vector<bool>& in, std::vector<bool>& out);
};

class OrGate : public BaseComponent {
  public:
    OrGate(std::vector<unsigned int> nodes)
      : BaseComponent("Or", nodes) {}

    void step(std::vector<bool>& in, std::vector<bool>& out);
};

class XorGate : public BaseComponent {
  public:
    XorGate(std::vector<unsigned int> nodes)
      : BaseComponent("Xor", nodes) {}

    void step(std::vector<bool>& in, std::vector<bool>& out);
};

class NandGate : public BaseComponent {
  public:
    NandGate(std::vector<unsigned int> nodes)
      : BaseComponent("Nand", nodes) {}

    void step(std::vector<bool>& in, std::vector<bool>& out);
};


//In lieu of proper reflection, this will have to do
//Maps strings to a (sort of) factory for components
//  Usage:
//    component_map componentMap;
//    componentMap["and"] = &createComponent<AndGate>;
//
//    BaseComponent * gate = componentMap["and"](nodes)
//    gate->step(a, b);
//    etc..
template<typename T> BaseComponent * createComponent(std::vector<unsigned int> nodes) {
  return new T(nodes);
}

typedef std::map<std::string, BaseComponent*(*)(std::vector<unsigned int>)> component_map;

#endif
