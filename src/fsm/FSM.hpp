#pragma once

#include "State.hpp"
#include "Transition.hpp"
#include "Variable.hpp"
#include <string>
#include <utility> // std::pair

struct PairHash
{
    template <class T1, class T2> std::size_t operator()(const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
    }
};

class FSM
{
  private:
    std::string name;
    std::string comment;

    State initial_state;

    std::unordered_map<std::string, State> states;
    std::unordered_map<std::pair<std::string, std::string>, Transition, PairHash> transitions;
    std::vector<Variable> variables;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

  public:
    FSM();
    FSM(std::string name);
    FSM(std::string name, std::string comment);

    void prettyPrint();

    std::string getName();
    std::string getComment();
    State getInitialState();
    std::unordered_map<std::string, State> getStates();
    State getState(std::string state_name);
    std::unordered_map<std::pair<std::string, std::string>, Transition, PairHash> getTransitions();
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
