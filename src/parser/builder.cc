#include "builder.h"

namespace Builder {
  Network * build(std::string filepath) {
    Definition * def = Parser::parseDefinition(filepath);

    Network * net = buildNetwork(def);

    return net;
  }

  Network * buildNetwork(Definition * def) {
    LOG_DEBUG << "Building network";

    Network * net = new Network();

    std::map<std::string, Network*> includes;
    LOG_DEBUG << "Recursing into includes";
    makeIncludes(def, includes);

    LOG_DEBUG << "Adding Network IO";
    addIO(net, def);

    LOG_DEBUG << "Adding components";
    addComponents(net, def);

    LOG_DEBUG << "Connecting components";
    connectComponents(net, def);

    return net;
  }

  void makeIncludes(Definition * def, std::map<std::string, Network*>& includes) {
    //Build each network in the includes and add to the includes map
    //for_each(include in def)
    //  includes[include_nickname] = build(include_path)

    if( def->pairs.find("includes") != def->pairs.end() ) {
      for(std::map<std::string, Definition*>::iterator it = def->pairs["includes"]->pairs.begin();
          it != def->pairs["includes"]->pairs.end();
          it++) {
        includes[it->first] = build(it->second->value);
      }
    }

    return;
  }

  void addIO(Network * net, Definition * def) {
    //Add all the named IO in def to net
    //for_each(element in def inputs/outputs)
    //  net->addInput/addOutput

    if( def->pairs.find("inputs") != def->pairs.end() ) {
      for(std::map<std::string, Definition*>::iterator it = def->pairs["inputs"]->pairs.begin();
          it != def->pairs["inputs"]->pairs.end();
          it++) {
        net->addInput( it->first );
      }
    }
    else {
      //TODO add a warning
    }

    if( def->pairs.find("outputs") != def->pairs.end() ) {
      //Add some outputs
      for(std::map<std::string, Definition*>::iterator it = def->pairs["outputs"]->pairs.begin();
          it != def->pairs["outputs"]->pairs.end();
          it++) {
        net->addOutput( it->first );
      }
    }
    else {
      //TODO add a warning
    }

    return;
  }

  void addComponents(Network * net, Definition * def) {
    //Add each component to the network
    //for_each(component_nickname in def)
    //  net->addComponent

    if( def->pairs.find("components") == def->pairs.end() ) {
      //TODO raise an error
      return;
    }
    for(std::map<std::string, Definition*>::iterator it = def->pairs["components"]->pairs.begin();
        it != def->pairs["components"]->pairs.end();
        it++) {
      std::string name = it->first;
      std::string type = it->second->pairs["type"]->value;

      net->addComponent(type, name);
    }

    return;
  }

  void connectComponents(Network * net, Definition * def) {
    //Make all the connections defined in def
    //for_each(component in def)
    //  for_each(input in component_inputs)
    //    connect the input to the thing

    if( def->pairs.find("outputs") != def->pairs.end() ) {
      //Connect any outputs
      for(std::map<std::string, Definition*>::iterator it = def->pairs["outputs"]->pairs.begin();
          it != def->pairs["outputs"]->pairs.end();
          it++) {
        std::pair<std::string, std::string> dest = Helpers::separateDotted( it->second->value );
        net->connect(dest.first, dest.second, "outputs", it->first);
      }
    }

    if( def->pairs.find("components") != def->pairs.end() ) {
      for(std::map<std::string, Definition*>::iterator it1 = def->pairs["components"]->pairs.begin();
          it1 != def->pairs["components"]->pairs.end();
          it1++) {
        //Iterating over each component
        if( it1->second->pairs.find("inputs") != it1->second->pairs.end() ) {
          //If the component has inputs
          for(std::map<std::string, Definition*>::iterator it2 = it1->second->pairs["inputs"]->pairs.begin();
              it2 != it1->second->pairs["inputs"]->pairs.end();
              it2++) {
            //Iterating over each input in a component
            std::pair<std::string, std::string> source = Helpers::separateDotted( it2->second->value );
            net->connect( source.first, source.second, it1->first, it2->first );
          }
        }
      }
    }

    return;
  }

  namespace Helpers {
    std::pair<std::string, std::string> separateDotted(std::string in) {
      std::pair<std::string, std::string> out;
      out.first = in.substr(0, in.find_first_of('.'));
      out.second = in.substr(in.find_first_of('.')+1);
      return out;
    }
  }
}
