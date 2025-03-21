#include "Transition.hpp"

Transition::Transition(State from, State to, std::function<bool()> condition, int delay)
    : from(from), to(to), condition(condition), delay(delay)
{
}

State Transition::getFrom()
{
    return from;
}

State Transition::getTo()
{
    return to;
}

std::function<bool()> Transition::getCondition()
{
    return condition;
}

int Transition::getDelay()
{
    return delay;
}

void Transition::setFrom(State new_from)
{
    from = new_from;
}

void Transition::setTo(State new_to)
{
    to = new_to;
}

void Transition::setCondition(std::function<bool()> new_condition)
{
    condition = new_condition;
}

void Transition::setDelay(int new_delay)
{
    delay = new_delay;
}