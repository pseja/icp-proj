#include "FSM.hpp"
#include <iostream>

FSM::FSM()
{
}

FSM::FSM(std::string name) : name(name)
{
}

FSM::FSM(std::string name, std::string comment) : name(name), comment(comment)
{
}

void FSM::prettyPrint()
{
    std::cout << "Name: " << name << "\n";
    std::cout << "Comment: " << comment << "\n";

    std::cout << "States:\n";
    for (State state : states)
    {
        if (state.getName() == initial_state.getName())
        {
            std::cout << " -> " << state.getName() << "\n";
        }
        else
        {
            std::cout << "    " << state.getName() << "\n";
        }
    }
}

std::string FSM::getName()
{
    return name;
}

std::string FSM::getComment()
{
    return comment;
}

State FSM::getInitialState()
{
    return initial_state;
}

std::vector<State> FSM::getStates()
{
    return states;
}

std::vector<Transition> FSM::getTransitions()
{
    return transitions;
}

std::vector<Variable> FSM::getVariables()
{
    return variables;
}

std::vector<std::string> FSM::getInputs()
{
    return inputs;
}

std::vector<std::string> FSM::getOutputs()
{
    return outputs;
}

void FSM::setName(std::string new_name)
{
    name = new_name;
}

void FSM::setComment(std::string new_comment)
{
    comment = new_comment;
}

void FSM::setInitialState(State new_initial_state)
{
    initial_state = new_initial_state;
}

void FSM::addState(State new_state)
{
    states.push_back(new_state);
}

void FSM::addTransition(Transition new_transition)
{
    transitions.push_back(new_transition);
}

void FSM::addVariable(Variable new_variable)
{
    variables.push_back(new_variable);
}

void FSM::addInput(std::string new_input)
{
    inputs.push_back(new_input);
}

void FSM::addOutput(std::string new_output)
{
    outputs.push_back(new_output);
}
