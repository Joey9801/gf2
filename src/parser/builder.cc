#include "builder.h"

namespace Builder {
  RootNetwork * buildRoot(std::string filepath) {
    Network * net = build(filepath);

    RootNetwork * rnet;
    rnet = new RootNetwork(net);
    rnet->setMonitor(new Monitor());

    try {
      createMonitorPoints(rnet);
    }
    catch(const ErrorList& e) {
      rnet->errorList.addList(e);
      if(not rnet->errorList.recoverable())
        throw;
    }

    return rnet;
  }
  Network * build(std::string filepath) {
    ErrorList errorList;
    Network * net;
    Definition * def;

    try {
      def = Parser::parseDefinition(filepath);
    }
    catch(const ErrorList& e) {
      throw;
    }
    catch(...) {
      LOG_ERROR << "Parsing \"" << filepath << "\" caused an unrecoverable error";
      throw;
    }

    def->filepath = filepath;
    try {
      net = buildNetwork(def);
    }
    catch(const ErrorList& e) {
      LOG_ERROR << "Encountered unrecoverable errorList while parsing \"" << filepath << "\"";

      std::vector<std::string> errors = errorList.formatErrors();
      for(std::vector<std::string>::iterator it = errors.begin();
          it != errors.end();
          it++) {
        LOG_ERROR << (*it);
      }
      throw;
    }
    catch(...) {
      throw;
    }

    return net;
  }

  Network * buildNetwork(Definition * def) {
    LOG_VERBOSE << "Building network";

    ErrorList errorList;
    Network * net = new Network();
    net->setDefinition(def);

    std::map<std::string, Network*> includes;
    LOG_VERBOSE << "Recursing into includes";

    try {
      makeIncludes(def, includes);
    }
    catch(const ErrorList& e) {
      errorList.addList(e);
      if(not errorList.recoverable()) {
        throw errorList;
      }
    }
    LOG_VERBOSE << "Finished making includes";

    LOG_VERBOSE << "Adding Network IO";
    try {
      addIO(net, def);
    }
    catch(const ErrorList& e) {
      errorList.addList(e);
      if(not errorList.recoverable()) {
        throw errorList;
      }
    }

    LOG_VERBOSE << "Adding components";
    try {
      addComponents(net, def, includes);
    }
    catch(const ErrorList& e) {
      errorList.addList(e);
      if(not errorList.recoverable()) {
        throw errorList;
      }
    }

    LOG_VERBOSE << "Configuring components";
    try {
      configureComponents(net, def);
    }
    catch(const ErrorList& e) {
      errorList.addList(e);
      if(not errorList.recoverable()) {
        throw errorList;
      }
    }

    LOG_VERBOSE << "Connecting components";
    try {
      connectComponents(net, def);
    }
    catch(const ErrorList& e) {
      errorList.addList(e);
      if(not errorList.recoverable()) {
        throw errorList;
      }
    }

    if(errorList.anyErrors()) {
      std::vector<std::string> errors = errorList.formatErrors();
      for(std::vector<std::string>::iterator it = errors.begin();
          it != errors.end();
          it++) {
        LOG_ERROR << (*it);
      }
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
        includes[it->second->value] = build(it->first);
      }
    }

    return;
  }

  void addIO(Network * net, Definition * def) {
    //Add all the named IO in def to net
    //for_each(element in def inputs/outputs)
    //  net->addInput/addOutput

    ErrorList errorList;

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
      ParseError * e = new ParseError();
      e->name = "Missing root node";
      e->detail = "\"inputs\" is missing";
      e->location.file = def->filepath;
      e->recoverable = true;
      errorList.addError(e);
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
      ParseError * e = new ParseError();
      e->name = "Missing root node";
      e->detail = "\"outputs\" is missing";
      e->location.file = def->filepath;
      e->recoverable = true;
      errorList.addError(e);
    }

    throw errorList;
  }

  void addComponents(Network * net, Definition * def, std::map<std::string, Network*>& includes) {
    //Add each component to the network
    //for_each(component_nickname in def)
    //  net->addComponent

    ErrorList errorList;

    if( def->pairs.find("components") == def->pairs.end() ) {
      ParseError * e = new ParseError();
      e->name = "Missing root node";
      e->detail = "\"components\" is missing";
      e->location.file = def->filepath;
      e->recoverable = false;
      errorList.addError(e);
      throw errorList;
    }
    for(std::map<std::string, Definition*>::iterator it = def->pairs["components"]->pairs.begin();
        it != def->pairs["components"]->pairs.end();
        it++) {

      if(it->second->pairs.find("type") == it->second->pairs.end()) {
        ParseError * e = new ParseError();
        e->name = "Field is missing";
        e->detail = "Component \"type\" field is missing";
        e->location.file = def->filepath;
        e->recoverable = false;
        errorList.addError(e);
        throw errorList;
      }

      std::string name = it->first;
      std::string type = it->second->pairs["type"]->value;

      try {
        if(type.find_first_of('.') != std::string::npos) {
          std::pair<std::string, std::string> value = Helpers::separateDotted(type);
          if( value.first == "includes" )
            net->addComponent(includes[value.second]->clone(), name);
        }
        else {
          net->addComponent(type, name);
        }
      }
      catch(GF2Error& e) {
        GF2Error * err = new GF2Error(e);
        errorList.addError(err);
        throw errorList;
      }
    }

    return;
  }

  void configureComponents(Network * net, Definition * def) {
    ErrorList errorList;

    for(std::map<std::string, Definition*>::iterator it1 = def->pairs["components"]->pairs.begin();
        it1 != def->pairs["components"]->pairs.end();
        it1++) {
      //Iterating over each component
      if( it1->second->pairs.find("config") != it1->second->pairs.end() ) {
        //If the component has a config
        for(std::map<std::string, Definition*>::iterator it2 = it1->second->pairs["config"]->pairs.begin();
            it2 != it1->second->pairs["config"]->pairs.end();
            it2++) {
          try {
            net->configureComponent( it1->first, it2->first, it2->second->value );
          }
          catch(const GF2Error& e) {
            errorList.addError(e);
            if(not errorList.recoverable())
              throw;
          }
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
    else {
      ParseError * e = new ParseError();
      e->name = "Field is missing";
      e->detail = "\"config\" field is missing";
      e->location.file = def->filepath;
      e->recoverable = true;
      errorList.addError(e);
    }


    throw errorList;
  }

  void connectComponents(Network * net, Definition * def) {
    //Make all the connections defined in def
    //for_each(component in def)
    //  for_each(input in component_inputs)
    //    connect the input to the thing

    ErrorList errorList;
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
          std::string name = it->first.substr(0, it->first.find_first_of('['));
          for(std::map<std::string, Definition*>::iterator it2 = it->second->pairs.begin();
              it2 != it->second->pairs.end();
              it2++) {
            ss.str("");
            ss << name << "[" << it2->first << "]";
            std::pair<std::string, std::string> dest = Helpers::separateDotted( it2->second->value );
            try {
              net->connect(dest.first, dest.second, "outputs", ss.str());
            }
            catch(const GF2Error& e) {
              errorList.addError(e);
              if(not errorList.recoverable())
                throw errorList;
            }
          }
        }
        else {
          std::pair<std::string, std::string> dest = Helpers::separateDotted( it->second->value );
          net->connect(dest.first, dest.second, "outputs", it->first);
        }
      }
    }
    else {
      ParseError * e = new ParseError();
      e->name = "Field is missing";
      e->detail = "\"config\" field is missing";
      e->recoverable = true;
      errorList.addError(e);
    }

    LOG_VERBOSE << "Connecting components";
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
          catch(const GF2Error& e) {
            errorList.addError(e);
            if(not errorList.recoverable())
              throw errorList;
          }
        }
      }
    }

    throw errorList;
  }

  void createMonitorPoints(RootNetwork * rnet) {
    ErrorList errorList;
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

        try {
          unsigned int pointId = rnet->addMonitorPoint(signature);
          rnet->getMonitor()->renamePoint(pointId, it->first);
        }
        catch(const GF2Error& e) {
          errorList.addError(e);
          if(not errorList.recoverable())
            throw errorList;
        }
      }
    }

    throw errorList;
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
