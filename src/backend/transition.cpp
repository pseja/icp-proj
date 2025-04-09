#include "transition.hpp"

Transition::Transition(State *from, State *to, QString condition, int delay)
    : from(from), to(to), condition(condition), delay(delay)
{
    if (delay > 0)
    {
        is_delayed_transition = true;
    }
}

State *Transition::getFrom()
{
    return from;
}

State *Transition::getTo()
{
    return to;
}

QString Transition::getCondition()
{
    return condition;
}

int Transition::getDelay()
{
    return delay;
}

bool Transition::isDelayedTransition()
{
    return is_delayed_transition;
}

void Transition::setFrom(State *new_from)
{
    from = new_from;
}

void Transition::setTo(State *new_to)
{
    to = new_to;
}

void Transition::setCondition(QString new_condition)
{
    condition = new_condition;
}

void Transition::setDelay(int new_delay)
{
    delay = new_delay;
    if (delay > 0)
    {
        is_delayed_transition = true;
    }
}

void Transition::setDelayedTransition(bool new_is_delayed_transition)
{
    is_delayed_transition = new_is_delayed_transition;
}
