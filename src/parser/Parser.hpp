#pragma once

#include "../../external/pugixml/pugixml.hpp"
#include "../fsm/FSM.hpp"
#include <string>

class Parser
{
  public:
    static bool XMLtoFSM(const std::string &file_path, FSM &fsm);
};
