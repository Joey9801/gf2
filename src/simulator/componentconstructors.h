#ifndef COMPONENTCONSTRUCTORS_H_
#define COMPONENTCONSTRUCTORS_H_

#include <string>
#include <map>

#include "basecomponent.h"
#include "boolgates.h"
#include "flipflops.h"
#include "signalgenerator.h"
#include "dummyio.h"

//In lieu of proper reflection, the following will have to do
//Maps strings to a (sort of) factory for components
//  Usage:
//    component_map componentMap;
//    componentMap["and"] = &createComponent<AndGate>;
//
//    BaseComponent * gate = componentMap["and"]()
//    //Set up in/output references
//    gate->step(a, b);
//    etc..
template<typename T> BaseComponent * createComponent() {
  return new T();
}

typedef std::map<std::string, BaseComponent*(*)(void)> constructor_map;


#endif
