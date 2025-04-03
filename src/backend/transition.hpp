#pragma once

#include "state.hpp"
#include <functional> // for std::function

class Transition
{
  private:
    State *from;
    State *to;
    std::function<bool()> condition = nullptr;
    int delay = -1;

    bool is_delayed_transition = false;

  public:
    Transition(State *from, State *to, std::function<bool()> condition, int delay = -1);

    State *getFrom();
    State *getTo();
    std::function<bool()> getCondition();
    int getDelay();
    bool isDelayedTransition();

    void setFrom(State *new_from);
    void setTo(State *new_to);
    void setCondition(std::function<bool()> new_condition);
    void setDelay(int new_delay);
    void setDelayedTransition(bool new_is_delayed_transition);
};
