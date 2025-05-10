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

bool FSM::removeState(QString state_name) {
    if (!state_map.contains(state_name)) {
        qWarning() << state_name << "can't be deleted because it doesn't exist";
        return false;
    }

    State *state = state_map.value(state_name);

    return removeState(state);
}

void FSM::addState(State *new_state, QString new_name) { state_map[new_name] = new_state; }

bool FSM::removeState(State *state) {
    if (!state) {
        qWarning() << "removeState: state is null";
        return false;
    }

    QString state_name = state->getName();

    if (state == initial_state) {
        qWarning() << "Removing initial state" << state_name;
        initial_state = nullptr;
    }

    removeTransitionsFrom(state_name);
    removeTransitionsTo(state_name);

    state_map.remove(state_name);
    delete state;

    qInfo() << "State" << state_name << "removed from FSM";

    return true;
}

void FSM::addInput(QString new_input) { inputs.insert(new_input); }

bool FSM::removeInput(QString input_name) {
    if (!inputs.contains(input_name)) {
        qWarning() << input_name << "can't be deleted because it doesn't exist";
        return false;
    }

    inputs.remove(input_name);
    qInfo() << "Input" << input_name << "removed from FSM";

    return true;
}

void FSM::addOutput(QString new_output) { outputs.insert(new_output); }

bool FSM::removeOutput(QString output_name) {
    if (!outputs.contains(output_name)) {
        qWarning() << output_name << "can't be deleted because it doesn't exist";
        return false;
    }

    outputs.remove(output_name);
    qInfo() << "Output" << output_name << "removed from FSM";

    return true;
}

void FSM::addVariable(QString variable_type, QString variable_name, QString variable_value) {
    Variable *new_variable = new Variable(variable_type, variable_name, QVariant(variable_value));
    variable_map[variable_name] = new_variable;
}

bool FSM::removeVariable(QString variable_name) {
    if (!variable_map.contains(variable_name)) {
        qWarning() << variable_name << "can't be deleted because it doesn't exist";
        return false;
    }

    Variable *variable = variable_map.value(variable_name);

    return removeVariable(variable);
}

void FSM::addVariable(Variable *new_variable) { variable_map[new_variable->getName()] = new_variable; }

bool FSM::removeVariable(Variable *variable) {
    if (!variable) {
        qWarning() << "removeVariable: variable is null";
        return false;
    }

    QString variable_name = variable->getName();

    bool variable_in_use = false;
    for (auto transition : from_transition_map.values()) {
        if (transition->getDelayVariableName() == variable_name) {
            variable_in_use = true;
            qWarning() << "Variable" << variable_name << "is used in a transition delay from"
                       << transition->getFrom()->getName() << "to" << transition->getTo()->getName();
            return false;
        }
    }

    variable_map.remove(variable_name);
    delete variable;

    qInfo() << "Removed" << variable_name << "from FSM";

    return true;
}

void FSM::addTransition(Transition *new_transition) {
    from_transition_map.insert(new_transition->getFrom()->getName(), new_transition);
    to_transition_map.insert(new_transition->getTo()->getName(), new_transition);
}

bool FSM::removeTransition(Transition *transition) {
    if (!transition) {
        qWarning() << "removeTransition: transition is null";
        return false;
    }

    QString from_state_name = transition->getFrom()->getName();
    QString to_state_name = transition->getTo()->getName();
    QString event = transition->getEvent();

    bool from_removed = from_transition_map.remove(from_state_name, transition) > 0;
    bool to_removed = to_transition_map.remove(to_state_name, transition) > 0;

    if (from_removed && to_removed) {
        delete transition;
        qInfo() << "Transition from" << from_state_name << "to" << to_state_name
                << (event.isEmpty() ? "" : "with event " + event) << "removed from FSM";

        return true;
    } else {
        qWarning() << "Failed to remove transition completely";

        return false;
    }
}

void FSM::addTransition(State *from, State *to, QString event, QString condition, int delay,
                        QString delay_variable_name) {
    Transition *new_transition = new Transition(from, to, event, condition, delay, delay_variable_name);
    from_transition_map.insert(from->getName(), new_transition);
    to_transition_map.insert(to->getName(), new_transition);
}

int FSM::removeTransitionsBetween(State *from, State *to) {
    if (!from) {
        qWarning() << "removeTransitionsBetween: from state is null";
        return 0;
    }
    if (!to) {
        qWarning() << "removeTransitionsBetween: to state is null";
        return 0;
    }

    QString from_state_name = from->getName();
    QString to_state_name = to->getName();

    return removeTransitionsBetween(from_state_name, to_state_name);
}

int FSM::removeTransitionsBetween(QString from_state, QString to_state) {
    QList<Transition *> transitions = from_transition_map.values(from_state);

    QList<Transition *> transitions_to_remove;

    for (auto transition : transitions) {
        if (transition->getTo()->getName() == to_state) {
            transitions_to_remove.append(transition);
        }
    }

    int count = 0;
    for (auto transition : transitions_to_remove) {
        if (removeTransition(transition)) {
            count++;
        }
    }

    return count;
}

int FSM::removeTransitionsFrom(QString state_name) {
    QList<Transition *> transitions = from_transition_map.values(state_name);

    QList<Transition *> transitions_to_remove = transitions;

    int count = 0;
    for (auto transition : transitions_to_remove) {
        if (removeTransition(transition)) {
            count++;
        }
    }

    return count;
}

int FSM::removeTransitionsFrom(State *state) {
    if (!state) {
        qWarning() << "removeTransitionsFrom: state is null";
        return 0;
    }

    QString state_name = state->getName();

    return removeTransitionsFrom(state_name);
}

int FSM::removeTransitionsTo(QString state_name) {
    QList<Transition *> transitions = to_transition_map.values(state_name);

    QList<Transition *> transitions_to_remove = transitions;

    int count = 0;
    for (auto transition : transitions_to_remove) {
        if (removeTransition(transition)) {
            count++;
        }
    }

    return count;
}

int FSM::removeTransitionsTo(State *state) {
    if (!state) {
        qWarning() << "removeTransitionsTo: state is null";
        return 0;
    }

    QString state_name = state->getName();

    return removeTransitionsTo(state_name);
}
