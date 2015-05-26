#include "builder.h"

namespace Builder {
  RootNetwork * buildRoot(std::string filepath) {
    Network * net = build(filepath);

    RootNetwork * rnet;
    rnet = new RootNetwork(net);
    rnet->setMonitor(new Monitor());
    createMonitorPoints(rnet);

    return rnet;
  }
  Network * build(std::string filepath) {
    Definition * def = Parser::parseDefinition(filepath);

    Network * net;
    try {
      net = buildNetwork(def);
    }
    catch(...) {
      LOG_ERROR << filepath;
      throw;
    }

    return net;
  }

  Network * buildNetwork(Definition * def) {
    LOG_VERBOSE << "Building network";

    Network * net = new Network();
    net->setDefinition(def);

    std::map<std::string, Network*> includes;
    LOG_VERBOSE << "Recursing into includes";

    try {
      makeIncludes(def, includes);
    }
    catch(...) {
      LOG_ERROR << "Exception raised while making includes";
      throw;
    }
    LOG_VERBOSE << "Finished making includes";

    LOG_VERBOSE << "Adding Network IO";
    try {
      addIO(net, def);
    }
    catch(...) {
      LOG_ERROR << "Exception raised while adding network IO";
      throw;
    }

    LOG_VERBOSE << "Adding components";
    try {
    addComponents(net, def, includes);
    }
    catch(...) {
      LOG_ERROR << "Exception raised while adding components";
      throw;
    }

    LOG_VERBOSE << "Configuring components";
    try {
      configureComponents(net, def);
    }
    catch(...) {
      LOG_ERROR << "Exception raised while configuring components";
      throw;
    }

    LOG_VERBOSE << "Connecting components";
    try {
    connectComponents(net, def);
    }
    catch(...) {
      LOG_ERROR << "Exception raised while making connections";
      throw;
    }

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
        LOG_DEBUG << "Making: " << it->first;
        includes[it->second->value] = build(it->first);
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
        if((it->first.find_first_of('[') != std::string::npos)
            and (it->first.find_first_of(']') != std::string::npos))
          net->addVectorInput( it->first );
        else
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
        if((it->first.find_first_of('[') != std::string::npos)
            and (it->first.find_first_of(']') != std::string::npos))
          net->addVectorOutput( it->first );
        else
          net->addOutput( it->first );
      }
    }
    else {
      //TODO add a warning
    }

    return;
  }

  void addComponents(Network * net, Definition * def, std::map<std::string, Network*>& includes) {
    //Add each component to the network
    //for_each(component_nickname in def)
    //  net->addComponent

    if( def->pairs.find("components") == def->pairs.end() ) {
      //TODO raise an error
      throw 1;
    }
    for(std::map<std::string, Definition*>::iterator it = def->pairs["components"]->pairs.begin();
        it != def->pairs["components"]->pairs.end();
        it++) {
      std::string name = it->first;
      std::string type = it->second->pairs["type"]->value;

      if(type.find_first_of('.') != std::string::npos) {
        std::pair<std::string, std::string> value = Helpers::separateDotted(type);
        if( value.first == "includes" )
          net->addComponent(includes[value.second]->clone(), name);
      }
      else {
        net->addComponent(type, name);
      }
    }

    return;
  }

  void configureComponents(Network * net, Definition * def) {
    for(std::map<std::string, Definition*>::iterator it1 = def->pairs["components"]->pairs.begin();
        it1 != def->pairs["components"]->pairs.end();
        it1++) {
      //Iterating over each component
      if( it1->second->pairs.find("config") != it1->second->pairs.end() ) {
        //If the component has a config
        for(std::map<std::string, Definition*>::iterator it2 = it1->second->pairs["config"]->pairs.begin();
            it2 != it1->second->pairs["config"]->pairs.end();
            it2++) {
          net->configureComponent( it1->first, it2->first, it2->second->value );
        }
      }
    }

    //Configure the current network
    if( def->pairs.find("config") != def->pairs.end() ) {
      for(std::map<std::string, Definition*>::iterator it = def->pairs["config"]->pairs.begin();
          it != def->pairs["config"]->pairs.end();
          it++) {
        net->configure(it->first, it->second->value);
      }
    }


  }

  void connectComponents(Network * net, Definition * def) {
    //Make all the connections defined in def
    //for_each(component in def)
    //  for_each(input in component_inputs)
    //    connect the input to the thing

    LOG_VERBOSE << "Connecting outputs";
    if( def->pairs.find("outputs") != def->pairs.end() ) {
      //Connect any outputs
      for(std::map<std::string, Definition*>::iterator it = def->pairs["outputs"]->pairs.begin();
          it != def->pairs["outputs"]->pairs.end();
          it++) {
        if((it->first.find_first_of('[') != std::string::npos)
            and (it->first.find_first_of(']') != std::string::npos)) {
          // Iterate through the vector initialisation
          std::stringstream ss;
          LOG_DEBUG << "it->first: " << it->first;
          std::string name = it->first.substr(0, it->first.find_first_of('['));
          for(std::map<std::string, Definition*>::iterator it2 = it->second->pairs.begin();
              it2 != it->second->pairs.end();
              it2++) {
            ss.str("");
            ss << name << "[" << it2->first << "]";
            std::pair<std::string, std::string> dest = Helpers::separateDotted( it2->second->value );
            net->connect(dest.first, dest.second, "outputs", ss.str());
          }
        }
        else {
          std::pair<std::string, std::string> dest = Helpers::separateDotted( it->second->value );
          net->connect(dest.first, dest.second, "outputs", it->first);
        }
      }
    }

    LOG_VERBOSE << "Connecting components";
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
            std::string pinOut = source.second;
            std::string pinIn = it2->first;


            //If the IO name is a vector, strip the "[]" from the end
            if( it2->first.length() > 2 )
              if( it2->first.substr( it2->first.length()-2) == "[]" )
                pinIn = it2->first.substr(0, it2->first.size()-2);
            if( source.second.length() > 2 )
              if( source.second.substr( source.second.length()-2) == "[]" )
                pinOut = source.second.substr(0, source.second.size()-2);

            try {
              net->connect( source.first, pinOut, it1->first, pinIn );
            }
            catch(...) {
              LOG_ERROR << "net->connect(" << source.first << ", " << pinOut << ", "
                << it1->first << ", " << pinIn << ")";
              throw;
            }
          }
        }
      }
      LOG_DEBUG << "Finished making " << net->numConnections << " connections";
    }
    else {
      //This should already have been checked, but throw anyway just in case
      throw 1;
    }

    return;
  }

  void createMonitorPoints(RootNetwork * rnet) {
    Definition * def = rnet->getDefinition();

    if(def->pairs.find("monitor") != def->pairs.end()) {
      std::vector<std::string> signature;
      for(std::map<std::string, Definition*>::iterator it = def->pairs["monitor"]->pairs.begin();
          it != def->pairs["monitor"]->pairs.end();
          it++) {
        std::pair<std::string, std::string> dest = Helpers::separateDotted(it->second->value);
        signature.clear();
        signature.push_back(dest.second);
        signature.push_back(dest.first);

        unsigned int pointId = rnet->addMonitorPoint(signature);
        rnet->getMonitor()->renamePoint(pointId, it->first);
      }
    }
    return;
  }

  namespace Helpers {
    std::pair<std::string, std::string> separateDotted(std::string in) {
      std::pair<std::string, std::string> out;
      if(in.find_first_of('.') == std::string::npos) {
        out.first = in;
        out.second = std::string("");
      }
      else {
        out.first = in.substr(0, in.find_first_of('.'));
        out.second = in.substr(in.find_first_of('.')+1);
      }

      return out;
    }
  }
}
