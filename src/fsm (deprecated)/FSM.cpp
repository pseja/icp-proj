#include "fsm.hpp"
#include "logger.hpp"

FSM::FSM(std::string name) : QStateMachine(), name(name)
{
}

FSM::FSM(std::string name, std::string comment) : QStateMachine(), name(name), comment(comment)
{
}