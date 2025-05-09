/**
 * @file fsm.cpp
 * @brief Implements the FSM class and it's methods, which represent a finite state machine.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 04-04-2025
 */

#include "fsm.hpp"

#include "logger.hpp"

FSM::FSM(QObject *parent) : QStateMachine(parent) {}

FSM::FSM(QString name) : QStateMachine(), name(name) {}

FSM::FSM(QString name, QString comment) : QStateMachine(), name(name), comment(comment) {}

QString FSM::getName() { return name; }

QString FSM::getComment() { return comment; }

State *FSM::getInitialState() { return initial_state; }

State *FSM::getState(QString state_name) {
    if (state_map.find(state_name) == state_map.end()) {
        qWarning() << "State " << state_name << " not found";
        return nullptr;
    }
    if (state_map[state_name] == nullptr) {
        qWarning() << "State " << state_name << " is null";
        return nullptr;
    }

    return state_map[state_name];
}

Variable *FSM::getVariable(QString variable_name) {
    if (variable_map.find(variable_name) == variable_map.end()) {
        qWarning() << "Variable " << variable_name << " not found";
        return nullptr;
    }
    if (variable_map[variable_name] == nullptr) {
        qWarning() << "Variable " << variable_name << " is null";
        return nullptr;
    }

    return variable_map[variable_name];
}

QSet<QString> FSM::getInputs() { return inputs; }

QSet<QString> FSM::getOutputs() { return outputs; }

QMap<QString, Variable *> FSM::getVariables() { return variable_map; }

QMap<QString, State *> FSM::getStates() { return state_map; }

QMultiMap<QString, Transition *> FSM::getTransitions() { return from_transition_map; }

QMultiMap<QString, Transition *> FSM::getTransitionsFrom() { return from_transition_map; }

QMultiMap<QString, Transition *> FSM::getTransitionsTo() { return to_transition_map; }

QList<Transition *> FSM::getTransitionsFrom(State *state) { return from_transition_map.values(state->getName()); }

QList<Transition *> FSM::getTransitionsTo(State *state) { return to_transition_map.values(state->getName()); }

QList<Transition *> FSM::getTransitionsFrom(QString state_name) { return from_transition_map.values(state_name); }

QList<Transition *> FSM::getTransitionsTo(QString state_name) { return to_transition_map.values(state_name); }

QString FSM::getInitialFSMXML() { return initial_fsm_xml; }

void FSM::setName(QString new_name) { name = new_name; }

void FSM::setComment(QString new_comment) { comment = new_comment; }

void FSM::setInitialState(State *new_initial_state) { initial_state = new_initial_state; }

void FSM::setInitialFSMXML(QString new_initial_fsm_xml) { initial_fsm_xml = new_initial_fsm_xml; }

void FSM::addState(State *new_state) { state_map[new_state->getName()] = new_state; }

void FSM::addState(State *new_state, QString new_name) { state_map[new_name] = new_state; }

void FSM::addInput(QString new_input) { inputs.insert(new_input); }

void FSM::addOutput(QString new_output) { outputs.insert(new_output); }

void FSM::addVariable(QString variable_type, QString variable_name, QString variable_value) {
    Variable *new_variable = new Variable(variable_type, variable_name, QVariant(variable_value));
    variable_map[variable_name] = new_variable;
}

void FSM::addVariable(Variable *new_variable) { variable_map[new_variable->getName()] = new_variable; }

void FSM::addTransition(Transition *new_transition) {
    from_transition_map.insert(new_transition->getFrom()->getName(), new_transition);
    to_transition_map.insert(new_transition->getTo()->getName(), new_transition);
}

void FSM::addTransition(State *from, State *to, QString event, QString condition, int delay,
                        QString delay_variable_name) {
    Transition *new_transition = new Transition(from, to, event, condition, delay, delay_variable_name);
    from_transition_map.insert(from->getName(), new_transition);
    to_transition_map.insert(to->getName(), new_transition);
}
