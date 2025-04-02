#include "FiniteStateMachine.hpp"
#include <chrono>
#include <iostream>
#include <thread>

FiniteStateMachine::FiniteStateMachine(State initial) : current_state(initial)
{
}

void FiniteStateMachine::addState(State name, Action on_enter, Action on_exit)
{
    states[name] = {on_enter, on_exit, {}};
}

void FiniteStateMachine::addTransition(State from, State to, Transition condition)
{
    states[from].transitions[to] = condition;
}

void FiniteStateMachine::process()
{
    for (auto &[next_state, condition] : states[current_state].transitions)
    {
        if (condition())
        {
            changeState(next_state);
            return;
        }
    }
}

void FiniteStateMachine::run(int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        process();
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void FiniteStateMachine::changeState(State new_state)
{
    if (current_state == new_state)
    {
        return;
    }

    if (states[current_state].onExit)
    {
        states[current_state].onExit();
    }

    current_state = new_state;

    if (states[new_state].onEnter)
    {
        states[new_state].onEnter();
    }

    std::cout << "Transitioned to: " << new_state << "\n";
}
