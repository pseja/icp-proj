#include "State.hpp"

State::State() : name("")
{
}

State::State(std::string name) : name(name)
{
}

std::string State::getName()
{
    return name;
}

void State::setName(std::string new_name)
{
    name = new_name;
}
