#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <vector>
#include <string>
#include <map>
#include <sstream>

#include <plog/Log.h>

#include "../errors/error.h"
#include "../structures/nodetree.h"

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
    std::vector<unsigned int> getOutputVectorNodes(unsigned int pinOut);
    std::vector<unsigned int> getOutputVectorNodes(std::string name);
    unsigned int numOutputs(void);

    unsigned int getInputNode(unsigned int pinIn);
    unsigned int getInputNode(std::string name);
    std::vector<unsigned int> getInputVectorNodes(unsigned int pinIn);
    std::vector<unsigned int> getInputVectorNodes(std::string name);
    unsigned int numInputs(void);

    virtual void connectOutput(unsigned int outputId, unsigned int node);
    virtual void connectOutput(std::string name, unsigned int node);
    virtual void connectInput(unsigned int inputId, unsigned int node);
    virtual void connectInput(std::string name, unsigned int node);
    void connectVectorInput(unsigned int inputId, std::vector<unsigned int> nodes);
    void connectVectorInput(std::string name, std::vector<unsigned int> nodes);

    bool isInputVector(unsigned int inputId);
    bool isOutputVector(unsigned int outputId);
    bool isInputVector(std::string inputName);
    bool isOutputVector(std::string outputName);

    virtual void configure(std::string key, std::string value) {}

    virtual NodeTree * getNodeTree(void);

    virtual BaseComponent * clone(void);
 
    virtual void Reset(){};
  protected:
    std::string _name;

    pin_map _pinInMap;
    pin_map _pinOutMap;

    std::vector<unsigned int> _inputs;
    std::vector<unsigned int> _outputs;

    std::map<std::string, unsigned int> _inputVectors;
    std::map<std::string, unsigned int> _outputVectors;
};

#endif
