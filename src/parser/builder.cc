#include "builder.h"

namespace Builder {
  std::string reservedNames[] = {"const", "inputs", "outputs", "true", "false"};

  RootNetwork * buildRoot(std::string filepath) {
    Network * net = new Network();
    try {
      net = build(filepath);
    }
    catch(ErrorList * e) {
      if(not e->recoverable())
        throw;
    }
    catch(...) {
      LOG_ERROR << "Builder threw an unexpected error on " << filepath;
      throw;
    }


    RootNetwork * rnet;
    rnet = new RootNetwork(net);
    rnet->setMonitor(new Monitor());

    try {
      createMonitorPoints(rnet);
    }
    catch(ErrorList * e) {
      rnet->errorList->addList(e);
      if(not rnet->errorList->recoverable())
        throw &rnet->errorList;
    }

    try {
      setInitialInputs(rnet);
    }
    catch(ErrorList * e) {
      rnet->errorList->addList(e);
      if(not rnet->errorList->recoverable())
        throw &rnet->errorList;
    }

    return rnet;
  }

  Network * build(std::string filepath) {
    ErrorList * errorList = new ErrorList();;
    Network * net;
    Definition * def;
    Parser parser;
    std::vector<ParserError> errors;

    static unsigned int depth = 0;
    depth++;
    if(depth > 100) {
      BuildError * e = new BuildError();
      e->name = "BuildError";
      e->detail = "Include depth limit (100) reached. Have you accidentally included a file in itself?";
      e->recoverable = false;
      e->location.file = filepath;
      errorList->addError(e);
      throw errorList;
    }


    try {
      def = parser.parse(filepath, errors);
      if(errors.size() > 0) {
        for(std::vector<ParserError>::iterator it = errors.begin();
            it != errors.end();
            it++) {
          ParseError * e = new ParseError();
          e->name = "ParseError";
          e->detail = it->formatError();
          e->location.file = filepath;
          e->location.line = it->getLineNo();
          e->location.column = it->getCharNo();
          e->recoverable = false;

          errorList->addError(e);
        }
        throw errorList;
      }
    }
    catch(...) {
      LOG_ERROR << "Parsing \"" << filepath << "\" caused an unrecoverable error";
      throw;
    }

    def->filepath = filepath;
    try {
      net = buildNetwork(def);
    }
    catch(ErrorList * e) {
      LOG_ERROR << "Encountered unrecoverable errorList while parsing \"" << filepath << "\"";

      std::vector<std::string> errors = errorList->formatErrors();
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

    depth--;
    return net;
  }

  Network * buildNetwork(Definition * def) {
    LOG_VERBOSE << "Building network";

    ErrorList * errorList = new ErrorList();;
    Network * net = new Network();
    net->setDefinition(def);

    std::map<std::string, Network*> includes;
    LOG_VERBOSE << "Recursing into includes";

    try {
      makeIncludes(def, includes);
    }
    catch(ErrorList * e) {
      errorList->addList(e);
      if(not errorList->recoverable()) {
        throw errorList;
      }
    }
    LOG_VERBOSE << "Finished making includes";

    LOG_VERBOSE << "Adding Network IO";
    try {
      addIO(net, def);
    }
    catch(ErrorList * e) {
      errorList->addList(e);
      if(not errorList->recoverable()) {
        throw errorList;
      }
    }

    LOG_VERBOSE << "Adding components";
    try {
      addComponents(net, def, includes);
    }
    catch(ErrorList * e) {
      errorList->addList(e);
      if(not errorList->recoverable()) {
        throw errorList;
      }
    }

    LOG_VERBOSE << "Configuring components";
    try {
      configureComponents(net, def);
    }
    catch(ErrorList * e) {
      errorList->addList(e);
      if(not errorList->recoverable()) {
        throw errorList;
      }
    }

    LOG_VERBOSE << "Connecting components";
    try {
      connectComponents(net, def);
    }
    catch(ErrorList * e) {
      errorList->addList(e);
      if(not errorList->recoverable()) {
        throw errorList;
      }
    }

    if(errorList->anyErrors()) {
      std::vector<std::string> errors = errorList->formatErrors();
      for(std::vector<std::string>::iterator it = errors.begin();
          it != errors.end();
          it++) {
        LOG_ERROR << (*it);
      }
    }

    net->errorList = errorList;
    return net;
  }

  void makeIncludes(Definition * def, std::map<std::string, Network*>& includes) {
    //Build each network in the includes and add to the includes map
    //for_each(include in def)
    //  includes[include_nickname] = build(include_path)

    ErrorList * errorList = new ErrorList();

    if( def->pairs.find("includes") != def->pairs.end() ) {
      for(std::map<std::string, Definition*>::iterator it = def->pairs["includes"]->pairs.begin();
          it != def->pairs["includes"]->pairs.end();
          it++) {
        try {
          includes[it->second->value] = build(it->first);
        }
        catch(ErrorList * e) {
          errorList->addList(e);
          if(not errorList->recoverable())
            throw errorList;
        }

        errorList->addList(includes[it->second->value]->errorList);
      }
    }

    throw errorList;
  }

  void addIO(Network * net, Definition * def) {
    //Add all the named IO in def to net
    //for_each(element in def inputs/outputs)
    //  net->addInput/addOutput

    ErrorList * errorList = new ErrorList();;

    if( def->pairs.find("inputs") != def->pairs.end() ) {
      for(std::map<std::string, Definition*>::iterator it = def->pairs["inputs"]->pairs.begin();
          it != def->pairs["inputs"]->pairs.end();
          it++) {

        std::string * p;
        if((p = std::find(reservedNames, reservedNames+5, it->first)) != reservedNames+5) {
          BuildError * e = new BuildError();
          e->name = "Reserved word used as name";
          e->detail = "Reserved word \""+*p+"\" cannot be used as input name";
          e->location.file = def->filepath;
          e->recoverable = false;
          errorList->addError(e);
          throw errorList;
        }
        try {
          if((it->first.find_first_of('[') != std::string::npos)
              and (it->first.find_first_of(']') != std::string::npos))
            net->addVectorInput( it->first );
          else
            net->addInput( it->first );
        }
        catch(GF2Error& err) {
          LOG_DEBUG;
          BuildError * e = new BuildError();
          e->name = "Duplicated input";
          e->detail = "Network input \"" + it->first + "\" is declared multiple times";
          e->location.file = def->filepath;
          e->recoverable = false;
          errorList->addError(e);
        }

      }
    }
    else {
      BuildError * e = new BuildError();
      e->name = "Missing root node";
      e->detail = "\"inputs\" is missing";
      e->location.file = def->filepath;
      e->recoverable = true;
      errorList->addError(e);
    }

    if( def->pairs.find("outputs") != def->pairs.end() ) {
      //Add some outputs
      for(std::map<std::string, Definition*>::iterator it = def->pairs["outputs"]->pairs.begin();
          it != def->pairs["outputs"]->pairs.end();
          it++) {
        std::string * p;
        if((p = std::find(reservedNames, reservedNames+5, it->first)) != reservedNames+5) {
          BuildError * e = new BuildError();
          e->name = "Reserved word used as name";
          e->detail = "Reserved word \""+*p+"\" cannot be used as output name";
          e->location.file = def->filepath;
          e->recoverable = false;
          errorList->addError(e);
          throw errorList;
        }
        try {
          if((it->first.find_first_of('[') != std::string::npos)
              and (it->first.find_first_of(']') != std::string::npos))
            net->addVectorOutput( it->first );
          else
            net->addOutput( it->first );
        }
        catch(GF2Error& err) {
          BuildError * e = new BuildError();
          e->name = "Duplicated input";
          e->detail = "Network input \"" + it->first + "\" is declared multiple times";
          e->location.file = def->filepath;
          e->recoverable = false;
          errorList->addError(e);
        }
      }
    }
    else {
      BuildError * e = new BuildError();
      e->name = "Missing root node";
      e->detail = "\"outputs\" is missing";
      e->location.file = def->filepath;
      e->recoverable = true;
      errorList->addError(e);
    }

    throw errorList;
  }

  void addComponents(Network * net, Definition * def, std::map<std::string, Network*>& includes) {
    //Add each component to the network
    //for_each(component_nickname in def)
    //  net->addComponent

    ErrorList * errorList = new ErrorList();;

    if( def->pairs.find("components") == def->pairs.end() ) {
      BuildError * e = new BuildError();
      e->name = "Missing root node";
      e->detail = "\"components\" is missing";
      e->location.file = def->filepath;
      e->recoverable = false;
      errorList->addError(e);
      throw errorList;
    }
    for(std::map<std::string, Definition*>::iterator it = def->pairs["components"]->pairs.begin();
        it != def->pairs["components"]->pairs.end();
        it++) {

      if(it->second->pairs.find("type") == it->second->pairs.end()) {
        BuildError * e = new BuildError();
        e->name = "Field is missing";
        e->detail = "Component \"type\" field is missing";
        e->location.file = def->filepath;
        e->recoverable = false;
        errorList->addError(e);
        throw errorList;
      }

      std::string * p;
      if((p = std::find(reservedNames, reservedNames+5, it->first)) != reservedNames+5) {
        BuildError * e = new BuildError();
        e->name = "Reserved word used as name";
        e->detail = "Reserved word \""+*p+"\" cannot be used as component name";
        e->location.file = def->filepath;
        e->recoverable = false;
        errorList->addError(e);
        throw errorList;
      }
      
      std::string name = it->first;
      std::string type = it->second->pairs["type"]->value;

      if(type.find_first_of('.') != std::string::npos) {
        std::pair<std::string, std::string> value = Helpers::separateDotted(type);
        if( value.first == "includes" ) {
          if(includes.find(value.second) == includes.end()) {
            BuildError * e = new BuildError();
            e->name = "Unknown component type";
            e->detail = "\"" + type +  "\" is not a known include";
            e->location.file = def->filepath;
            e->recoverable = false;
            errorList->addError(e);
            throw errorList;
          }
          net->addComponent(includes[value.second]->clone(), name);
        }
      }
      else {
        try {
          net->addComponent(type, name);
        }
        catch(GF2Error& e) {
          BuildError * e = new BuildError();
          e->name = "Unknown component type";
          e->detail = "\"" + type +  "\" is not a known component type";
          e->location.file = def->filepath;
          e->recoverable = false;
          errorList->addError(e);
          throw errorList;
        }
      }
    }

    return;
  }

  void configureComponents(Network * net, Definition * def) {
    ErrorList * errorList = new ErrorList();;

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
    else {
      BuildError * e = new BuildError();
      e->name = "Field is missing";
      e->detail = "\"config\" field is missing";
      e->location.file = def->filepath;
      e->recoverable = true;
      errorList->addError(e);
    }


    throw errorList;
  }

  void connectComponents(Network * net, Definition * def) {
    //Make all the connections defined in def
    //for_each(component in def)
    //  for_each(input in component_inputs)
    //    connect the input to the thing

    ErrorList * errorList = new ErrorList();;
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
            catch(const GF2Error& err) {
              LOG_DEBUG;
              BuildError * e = new BuildError();
              e->name = "Connection Error";
              e->detail = "Connecting  outputs." + ss.str() + " to " + dest.first + "." + dest.second;
              e->detail += "<br>" + err.detail;
              e->location.file = def->filepath;
              e->recoverable = false;
              errorList->addError(e);
              throw errorList;
            }
          }
        }
        else {
          std::pair<std::string, std::string> dest = Helpers::separateDotted( it->second->value );
          try {
            net->connect(dest.first, dest.second, "outputs", it->first);
          }
          catch(const GF2Error& err) {
            LOG_DEBUG;
            BuildError * e = new BuildError();
            e->name = "Connection Error";
            e->detail = "Connecting  outputs." + it->first + " to " + dest.first + "." + dest.second;
            e->detail += "<br>" + err.detail;
            e->location.file = def->filepath;
            e->recoverable = false;
            errorList->addError(e);
            throw errorList;
          }
        }
      }
    }
    else {
      BuildError * e = new BuildError();
      e->name = "Field is missing";
      e->detail = "\"config\" field is missing";
      e->location.file = def->filepath;
      e->recoverable = true;
      errorList->addError(e);
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
          catch(const GF2Error& err) {
            BuildError * e = new BuildError();
            e->name = "Connection Error";
            e->detail = "Connecting " + it1->first + "."  + pinIn + " to " + source.first + "." + source.second;
            e->detail += "<br>" + err.detail;
            e->location.file = def->filepath;
            e->recoverable = false;
            errorList->addError(e);
            throw errorList;
          }
        }
      }
    }

    throw errorList;
  }

  void createMonitorPoints(RootNetwork * rnet) {
    ErrorList * errorList = new ErrorList();;
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
          errorList->addError(e);
          if(not errorList->recoverable())
            throw errorList;
        }
      }
    }

    throw errorList;
  }

  void setInitialInputs(RootNetwork * rnet) {
    ErrorList * errorList = new ErrorList();;
    Definition * def = rnet->getDefinition();

    if(def->pairs.find("inputs") != def->pairs.end()) {
      for(std::map<std::string, Definition*>::iterator it = def->pairs["inputs"]->pairs.begin();
          it != def->pairs["inputs"]->pairs.end();
          it++) {
        if((it->first.find_first_of('[') != std::string::npos)
            and (it->first.find_first_of(']') != std::string::npos)) {
          //Handle vectors
          std::string name = it->first.substr(0, it->first.find_first_of('['));
          rnet->setDefaultVectorInput(name, (it->second->value=="true") );
        }
        else {
          //Regular inputs
          rnet->setInput(it->first, (it->second->value=="true") );
          rnet->setDefaultInput(it->first, (it->second->value=="true") );
        }
      }
    }
    // No error adding in an else statement, since a lack of inputs field
    // will have already been caught.

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
