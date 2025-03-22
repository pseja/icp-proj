#include "fsm/FSM.hpp"
#include "parser/Parser.hpp"
#include <iostream>

int main()
{
    FSM fsm;
    if (Parser::XMLtoFSM("./examples/TOF5s.xml", fsm))
    {
        std::cout << "FSM loaded successfully\n";
    }
    else
    {
        std::cerr << "Error loading FSM\n";
        return 1;
    }

    fsm.prettyPrint();

    if (Parser::FSMtoXML(fsm, "./examples/TOF5s_copy.xml"))
    {
        std::cout << "FSM saved successfully\n";
    }
    else
    {
        std::cerr << "Error saving FSM\n";
        return 2;
    }

    return 0;
}
