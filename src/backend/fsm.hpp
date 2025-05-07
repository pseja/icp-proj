/**
 * @file fsm.hpp
 * @brief Defines the FSM class, which represents a finite state machine.
 *
 * This file contains the declaration of the FSM class, which extends QStateMachine
 * and provides functionality for managing states, transitions, variables, inputs, and outputs.
 * The FSM class also supports querying transitions by their source or target states.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 04-04-2025
 */

#pragma once

#include "logger.hpp"
#include "state.hpp"
#include "transition.hpp"
#include "variable.hpp"
#include <QStateMachine>

/**
 * @class FSM
 * @brief Represents a finite state machine.
 *
 * The FSM class extends QStateMachine and provides additional functionality for managing
 * states, transitions, variables, inputs, and outputs. It supports querying transitions
 * by their source or target states and allows for dynamic state machine construction.
 */
class FSM : public QStateMachine
{
    Q_OBJECT

  private:
    QString name;    ///< The name of the finite state machine.
    QString comment; ///< An optional comment describing the finite state machine.

    QMap<QString, State *> state_map; ///< A map of state names to State objects.
    State *initial_state = nullptr;   ///< The initial state of the finite state machine.

    QMultiMap<QString, Transition *> from_transition_map; ///< Transitions grouped by their source state.
    QMultiMap<QString, Transition *> to_transition_map;   ///< Transitions grouped by their target state.

    QSet<QString> inputs;                   ///< A set of input names for the finite state machine.
    QSet<QString> outputs;                  ///< A set of output names for the finite state machine.
    QMap<QString, Variable *> variable_map; ///< A map of variable names to Variable objects.

  public:
    /**
     * @brief Constructs an FSM object with an optional parent.
     * @param parent The parent QObject, if any.
     */
    FSM(QObject *parent = nullptr);

    /**
     * @brief Constructs an FSM object with a name.
     * @param name The name of the finite state machine.
     */
    FSM(QString name);

    /**
     * @brief Constructs an FSM object with a name and a comment.
     * @param name The name of the finite state machine.
     * @param comment An optional comment describing the finite state machine.
     */
    FSM(QString name, QString comment);

    /**
     * @brief Gets the name of the finite state machine.
     * @return The name of the finite state machine.
     */
    QString getName();

    /**
     * @brief Gets the comment associated with the finite state machine.
     * @return The comment associated with the finite state machine.
     */
    QString getComment();

    /**
     * @brief Gets the initial state of the finite state machine.
     * @return A pointer to the initial State object.
     */
    State *getInitialState();

    /**
     * @brief Gets a state by its name.
     * @param state_name The name of the state.
     * @return A pointer to the State object, or nullptr if not found.
     */
    State *getState(QString state_name);

    /**
     * @brief Gets a variable by its name.
     * @param variable_name The name of the variable.
     * @return A pointer to the Variable object, or nullptr if not found.
     */
    Variable *getVariable(QString variable_name);

    /**
     * @brief Gets the set of input names.
     * @return A QSet containing the input names.
     */
    QSet<QString> getInputs();

    /**
     * @brief Gets the set of output names.
     * @return A QSet containing the output names.
     */
    QSet<QString> getOutputs();

    /**
     * @brief Gets the map of variables.
     * @return A QMap containing variable names and their corresponding Variable objects.
     */
    QMap<QString, Variable *> getVariables();

    /**
     * @brief Gets the map of states.
     * @return A QMap containing state names and their corresponding State objects.
     */
    QMap<QString, State *> getStates();

    /**
     * @brief Gets all transitions in the finite state machine.
     * @return A QMultiMap containing all transitions grouped by their source state.
     */
    QMultiMap<QString, Transition *> getTransitions();

    /**
     * @brief Gets all transitions originating from a specific state.
     * @return A QMultiMap containing transitions grouped by their source state.
     */
    QMultiMap<QString, Transition *> getTransitionsFrom();

    /**
     * @brief Gets all transitions targeting a specific state.
     * @return A QMultiMap containing transitions grouped by their target state.
     */
    QMultiMap<QString, Transition *> getTransitionsTo();

    /**
     * @brief Gets all transitions originating from a specific state.
     * @param state A pointer to the source state.
     * @return A QList containing the transitions originating from the specified state.
     */
    QList<Transition *> getTransitionsFrom(State *state);

    /**
     * @brief Gets all transitions targeting a specific state.
     * @param state A pointer to the target state.
     * @return A QList containing the transitions targeting the specified state.
     */
    QList<Transition *> getTransitionsTo(State *state);

    /**
     * @brief Gets all transitions originating from a state by its name.
     * @param state_name The name of the source state.
     * @return A QList containing the transitions originating from the specified state.
     */
    QList<Transition *> getTransitionsFrom(QString state_name);

    /**
     * @brief Gets all transitions targeting a state by its name.
     * @param state_name The name of the target state.
     * @return A QList containing the transitions targeting the specified state.
     */
    QList<Transition *> getTransitionsTo(QString state_name);

    /**
     * @brief Sets the name of the finite state machine.
     * @param new_name The new name of the finite state machine.
     */
    void setName(QString new_name);

    /**
     * @brief Sets the comment associated with the finite state machine.
     * @param new_comment The new comment for the finite state machine.
     */
    void setComment(QString new_comment);

    // TODO: stub
    QString getXml() const;
    /**
     * @brief Sets the initial state of the finite state machine.
     * @param new_initial_state A pointer to the new initial State object.
     */
    void setInitialState(State *new_initial_state);

    /**
     * @brief Adds a state to the finite state machine.
     * @param new_state A pointer to the State object to add.
     */
    void addState(State *new_state);

    /**
     * @brief Adds a state to the finite state machine with a specific name.
     * @param new_state A pointer to the State object to add.
     * @param new_name The name of the state.
     */
    void addState(State *new_state, QString new_name);

    /**
     * @brief Adds an input to the finite state machine.
     * @param new_input The name of the input to add.
     */
    void addInput(QString new_input);

    /**
     * @brief Adds an output to the finite state machine.
     * @param new_output The name of the output to add.
     */
    void addOutput(QString new_output);

    /**
     * @brief Adds a variable to the finite state machine.
     * @param new_variable A pointer to the Variable object to add.
     */
    void addVariable(Variable *new_variable);

    /**
     * @brief Adds a variable to the finite state machine with specific attributes.
     * @param new_variable_name The name of the variable.
     * @param new_variable_type The type of the variable.
     * @param new_variable_value The value of the variable.
     */
    void addVariable(QString new_variable_name, QString new_variable_type, QString new_variable_value);

    /**
     * @brief Adds a transition to the finite state machine.
     * @param new_transition A pointer to the Transition object to add.
     */
    void addTransition(Transition *new_transition);

    /**
     * @brief Adds a transition to the finite state machine with specific attributes.
     * @param from A pointer to the source state.
     * @param to A pointer to the target state.
     * @param event The event that triggers the transition.
     * @param condition The condition for the transition.
     * @param delay The delay (in milliseconds) before the transition occurs. Default is -1 (no delay).
     * @param delay_variable_name The name of the variable that holds the delay value.
     */
    void addTransition(State *from, State *to, QString event, QString condition, int delay = -1,
                       QString delay_variable_name = "");
};