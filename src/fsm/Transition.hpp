#pragma once

#include "State.hpp"
#include <functional>

class Transition
{
  private:
    State from;
    State to;
    // TODO needs more brainstorming with code gen
    std::function<bool()> condition;
    int delay = 0;

  public:
    Transition(State from, State to, std::function<bool()> condition, int delay = 0);

    State getFrom();
    State getTo();
    std::function<bool()> getCondition();
    int getDelay();

    void setFrom(State new_from);
    void setTo(State new_to);
    void setCondition(std::function<bool()> new_condition);
    void setDelay(int new_delay);
};
