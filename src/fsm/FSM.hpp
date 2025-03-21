#pragma once

#include "State.hpp"
#include "Transition.hpp"
#include "Variable.hpp"
#include <string>

class FSM
{
  private:
    std::string name;
    std::string comment;

    State initial_state;

    std::vector<State> states;
    std::vector<Transition> transitions;
    std::vector<Variable> variables;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

  public:
    FSM();
    FSM(std::string name);
    FSM(std::string name, std::string comment);

    std::string getName();
    std::string getComment();
    State getInitialState();
    std::vector<State> getStates();
    std::vector<Transition> getTransitions();
    std::vector<Variable> getVariables();
    std::vector<std::string> getInputs();
    std::vector<std::string> getOutputs();

    void setName(std::string new_name);
    void setComment(std::string new_comment);
    void setInitialState(State new_initial_state);
    void addState(State new_state);
    void addTransition(Transition new_transition);
    void addVariable(Variable new_variable);
    void addInput(std::string new_input);
    void addOutput(std::string new_output);
};
