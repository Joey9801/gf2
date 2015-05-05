#ifndef BOOLGATES_H_
#define BOOLGATES_H_

#include <string>
#include <vector>

#include "basecomponent.h"

class BaseBoolgate : public BaseComponent {
  public:
    BaseBoolgate(std::string name);
    virtual ~BaseBoolgate() {}

    void step(std::vector<bool>& a, std::vector<bool>& b);
    void connectInput(unsigned int index, unsigned int node);

  protected:
    virtual bool _compare(bool a, bool b) =0;
};

class AndGate : public BaseBoolgate {
  public:
    AndGate() : BaseBoolgate("And") {}
  protected:
    bool _compare(bool a, bool b);
};

class OrGate : public BaseBoolgate {
  public:
    OrGate() : BaseBoolgate("Or") {}
  protected:
    bool _compare(bool a, bool b);
};

class NandGate : public BaseBoolgate {
  public:
    NandGate() : BaseBoolgate("Nand") {}
  protected:
    bool _compare(bool a, bool b);
};

class NorGate : public BaseBoolgate {
  public:
    NorGate() : BaseBoolgate("Nor") {}
  protected:
    bool _compare(bool a, bool b);
};

#endif
