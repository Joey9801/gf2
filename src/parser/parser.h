#ifndef PARSER_H_
#define PARSER_H_

#include <string>

#include "../structures/definition.h"

namespace Parser {
  Definition * parseDefinition(std::string filepath);
}

#endif
