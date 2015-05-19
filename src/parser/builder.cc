#include "builder.h"

namespace Builder {
  Network * build(std::string filepath) {
    Definition * def = Parser::parseDefinition(filepath);

    Network * net = buildNetwork(def);

    return net;
  }

  Network * buildNetwork(Definition * def) {
    Network * net = new Network();

    std::map<std::string, Network*> includes;
    makeIncludes(def, includes);

    addIO(net, def);

    addComponents(net, def);

    connectComponents(net, def);

    return net;
  }

  void makeIncludes(Definition * def, std::map<std::string, Network*>& includes) {
    //Build each network in the includes and add to the includes map
    //for_each(include in def)
    //  includes[include_nickname] = build(include_path)
    return;
  }

  void addIO(Network * net, Definition * def) {
    //Add all the named IO in def to net
    //for_each(element in def inputs/outputs)
    //  net->addInput/addOutput
    return;
  }

  void addComponents(Network * net, Definition * def) {
    //Add each component to the network
    //for_each(component_nickname in def)
    //  net->addComponent
    return;
  }

  void connectComponents(Network * net, Definition * def) {
    //Make all the connections defined in def
    //for_each(component in def)
    //  for_each(input in component_inputs)
    //    connect the input to the thing
    return;
  }

  namespace Helpers {
    std::pair<std::string, std::string> separateDotten(std::string in);
  }
}
