#ifndef BUILDER_H_
#define BUILDER_H_

#include <string>
#include <vector>
#include <map>
#include <utility>

#include <plog/Log.h>

#include "../errors/error.h"
#include "parser.h"
#include "../structures/definition.h"
#include "../simulator/monitor.h"
#include "../simulator/network.h"
#include "../simulator/rootnetwork.h"

namespace Builder {
  RootNetwork * buildRoot(std::string filepath);

  Network * build(std::string filepath);

  Network * buildNetwork(Definition * def);

  void makeIncludes(Definition * def, std::map<std::string, Network*>& includes);
  void addIO(Network * net, Definition * def);
  void addComponents(Network * net, Definition * def, std::map<std::string, Network*>& includes);
  void configureComponents(Network * net, Definition * def);
  void connectComponents(Network * net, Definition * def);
  void createMonitorPoints(RootNetwork * rnet);
  void setInitialInputs(RootNetwork * rnet);

  namespace Helpers {
    std::pair<std::string, std::string> separateDotted(std::string in);
  }
}

#endif
