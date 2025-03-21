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

    std::cout << "    > FSM name: " << fsm.getName() << "\n";
    std::cout << "    > FSM comment: " << fsm.getComment() << "\n";

    return 0;
}
