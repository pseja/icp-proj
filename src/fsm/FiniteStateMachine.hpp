// DISCLAIMER:
// Don't use this code, it was written just for testing. It's equivalent is FSM.cpp

#ifndef FINITE_STATE_MACHINE_HPP
#define FINITE_STATE_MACHINE_HPP

#include <functional>
#include <string>
#include <unordered_map>

class FiniteStateMachine
{
  public:
    using State = std::string;
    using Transition = std::function<bool()>;
    using Action = std::function<void()>;

    struct StateInfo
    {
        Action onEnter;
        Action onExit;
        std::unordered_map<State, Transition> transitions;
    };

    FiniteStateMachine(State initial);

    std::string getName();
    void setName(std::string new_name);

    void addState(State name, Action on_enter, Action on_exit);
    void addTransition(State from, State to, Transition condition);
    void process();
    void run(int iterations = 10);

  private:
    std::string name;
    std::string comment;
    State initial_state;

    State current_state;
    std::unordered_map<State, StateInfo> states;
    void changeState(State new_state);
};

#endif // !FINITE_STATE_MACHINE_HPP
