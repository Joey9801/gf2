#ifndef BUILDER_H_
#define BUILDER_H_

#include <string>
#include <vector>
#include <map>
#include <utility>

#include <plog/Log.h>

#include "parser.h"
#include "../structures/definition.h"
#include "../simulator/network.h"

namespace Builder {
  Network * build(std::string filepath);

  Network * buildNetwork(Definition * def);

  void makeIncludes(Definition * def, std::map<std::string, Network*>& includes);
  void addIO(Network * net, Definition * def);
  void addComponents(Network * net, Definition * def);
  void connectComponents(Network * net, Definition * def);

  namespace Helpers {
    std::pair<std::string, std::string> separateDotted(std::string in);
  }
}

#endif
