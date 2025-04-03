#pragma once

#include "logger.hpp"
#include "state.hpp"
#include "variable.hpp"
#include <QEventTransition>
#include <QStateMachine>
#include <iostream>

class FSM : public QStateMachine
{
    Q_OBJECT

  private:
    QString name;
    QString comment;

    QMap<QString, State *> state_map;
    State *initial_state = nullptr;

    // TODO figure out transitions
    // QMap<QString, QEventTransition *> from_transition_map;
    // QMap<QString, QEventTransition *> to_transition_map;

    QSet<QString> inputs;
    QSet<QString> outputs;
    QMap<QString, Variable *> variable_map;

  public:
    FSM(QObject *parent = nullptr) : QStateMachine(parent)
    {
    }
    FSM(QString name);
    FSM(QString name, QString comment);

    QString getName();
    QString getComment();
    State *getInitialState();

    State *getState(QString state_name);
    Variable *getVariable(QString variable_name);
    // getTransition

    QSet<QString> getInputs();
    QSet<QString> getOutputs();
    QMap<QString, Variable *> getVariables();

    QMap<QString, State *> getStates();
    // QMap<QString, QEventTransition *> getTransitions();

    void setName(QString new_name);
    void setComment(QString new_comment);
    void setInitialState(State *new_initial_state);

    void addState(State *new_state);
    void addState(State *new_state, QString new_name);

    void addInput(QString new_input);
    void addOutput(QString new_output);
    void addVariable(Variable *new_variable);
    void addVariable(QString new_variable_name, QString new_variable_type, QString new_variable_value);
};
