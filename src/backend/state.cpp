/**
 * @file state.cpp
 * @brief Implements the State class, which represents a state in a state machine.
 *
 * This file contains the implementation of the State class, including methods
 * for managing state-specific attributes such as name, code, and whether the
 * state is an initial state.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#include "state.hpp"

State::State(QString name) : QState(), name(name)
{
}

QString State::getName()
{
    return name;
}

QString State::getCode()
{
    return code;
}

bool State::isInitial()
{
    return is_initial;
}

void State::setName(QString new_name)
{
    name = new_name;
}

void State::setCode(QString new_code)
{
    code = new_code;
}

void State::setInitial(bool new_is_initial)
{
    is_initial = new_is_initial;
}
