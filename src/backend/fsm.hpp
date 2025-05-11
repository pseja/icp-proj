/**
 * @file fsm.hpp
 * @brief Defines the FSM class and it's methods, which represent a finite state machine.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 04-04-2025
 */

#pragma once

#include <QStateMachine>

#include "state.hpp"
#include "transition.hpp"
#include "variable.hpp"

/**
 * @class FSM
 * @brief Represents a finite state machine (FSM).
 */
class FSM : public QStateMachine {
    Q_OBJECT

   private:
    QString name;     ///< The name of the FSM.
    QString comment;  ///< An optional comment describing the FSM.

    QMap<QString, State *> state_map;  ///< A map of state names to State objects.
    State *initial_state = nullptr;    ///< The initial state of the FSM.

    QMultiMap<QString, Transition *> from_transition_map;  ///< Transitions grouped by their source state.
    QMultiMap<QString, Transition *> to_transition_map;    ///< Transitions grouped by their target state.

    QSet<QString> inputs;                    ///< A set of input names for the FSM.
    QSet<QString> outputs;                   ///< A set of output names for the FSM.
    QMap<QString, Variable *> variable_map;  ///< A map of variable names to Variable objects.

    QString initial_fsm_xml;  ///< Stores the initial XML of the FSM.

   public:
    /**
     * @brief Constructs an FSM object.
     *
     * @param parent The parent object (optional).
     */
    FSM(QObject *parent = nullptr);

    /**
     * @brief Constructs an FSM object with a name.
     *
     * @param name The name of the FSM.
     */
    FSM(QString name);

    /**
     * @brief Constructs an FSM object with a name and a comment.
     *
     * @param name The name of the FSM.
     * @param comment An optional comment describing the FSM.
     */
    FSM(QString name, QString comment);

    /**
     * @brief Adds a state to the FSM.
     *
     * @param new_state A pointer to the State object to add.
     */
    void addState(State *new_state);

    /**
     * @brief Removes a state from the FSM.
     *
     * Also removes all transitions going from this state.
     *
     * @param state_name The name of the state to remove.
     *
     * @return True if the state was successfully removed, false otherwise.
     */
    bool removeState(QString state_name);

    /**
     * @brief Adds a state to the FSM with a specific name.
     *
     * @param new_state A pointer to the State object to add.
     * @param new_name The name of the state.
     */
    void addState(State *new_state, QString new_name);

    /**
     * @brief Removes a state from the FSM.
     *
     * Also removes all transitions going from this state.
     *
     * @param state A pointer to the State object to remove.
     *
     * @return True if the state was successfully removed, false otherwise.
     */
    bool removeState(State *state);

    /**
     * @brief Adds an input to the FSM.
     *
     * @param new_input The name of the input to add.
     */
    void addInput(QString new_input);

    /**
     * @brief Removes an input from the FSM.
     *
     * @param input_name The name of the input to remove.
     *
     * @return True if the input was successfully removed, false otherwise.
     */
    bool removeInput(QString input_name);

    /**
     * @brief Adds an output to the FSM.
     *
     * @param new_output The name of the output to add.
     */
    void addOutput(QString new_output);

    /**
     * @brief Removes an output from the FSM.
     *
     * @param output_name The name of the output to remove.
     *
     * @return True if the output was successfully removed, false otherwise.
     */
    bool removeOutput(QString output_name);

    /**
     * @brief Adds a variable to the FSM.
     *
     * @param new_variable A pointer to the Variable object to add.
     */
    void addVariable(Variable *new_variable);

    /**
     * @brief Removes a variable from the FSM.
     *
     * @param variable A pointer to the Variable object to remove.
     *
     * @return True if the variable was successfully removed, false otherwise.
     */
    bool removeVariable(Variable *variable);

    /**
     * @brief Adds a variable to the FSM with specific attributes.
     *
     * @param new_variable_name The name of the variable.
     * @param new_variable_type The type of the variable.
     * @param new_variable_value The value of the variable.
     */
    void addVariable(QString new_variable_name, QString new_variable_type, QString new_variable_value);

    /**
     * @brief Removes a variable from the FSM.
     *
     * @param variable_name The name of the variable to remove.
     *
     * @return True if the variable was successfully removed, false otherwise.
     */
    bool removeVariable(QString variable_name);

    /**
     * @brief Adds a transition to the FSM.
     *
     * @param new_transition A pointer to the Transition object to add.
     */
    void addTransition(Transition *new_transition);

    /**
     * @brief Removes a transition from the FSM.
     *
     * @param transition A pointer to the Transition object to remove.
     *
     * @return True if the transition was successfully removed, false otherwise.
     */
    bool removeTransition(Transition *transition);

    /**
     * @brief Adds a transition to the FSM with specific attributes.
     *
     * @param from A pointer to the source state.
     * @param to A pointer to the target state.
     * @param event The event that triggers the transition.
     * @param condition The condition for the transition.
     * @param delay The delay (in milliseconds) before the transition occurs. Default is -1 (no delay).
     * @param delay_variable_name The name of the variable that holds the delay value.
     */
    void addTransition(State *from, State *to, QString event, QString condition, int delay = -1,
                       QString delay_variable_name = "");

    /**
     * @brief Removes all transitions between two states.
     *
     * @param from_state The name of the source state.
     * @param to_state The name of the target state.
     *
     * @return The number of transitions removed.
     */
    int removeTransitionsBetween(QString from_state, QString to_state);

    /**
     * @brief Removes all transitions between two states.
     *
     * @param from The source state.
     * @param to The target state.
     *
     * @return The number of transitions removed.
     */
    int removeTransitionsBetween(State *from, State *to);

    /**
     * @brief Removes all transitions going from a specific state.
     *
     * @param state_name The name of the state.
     *
     * @return The number of transitions removed.
     */
    int removeTransitionsFrom(QString state_name);

    /**
     * @brief Removes all transitions going from a specific state.
     *
     * @param state The State object.
     *
     * @return The number of transitions removed.
     */
    int removeTransitionsFrom(State *state);

    /**
     * @brief Removes all transitions going to a specific state.
     *
     * @param state_name The name of the state.
     *
     * @return The number of transitions removed.
     */
    int removeTransitionsTo(QString state_name);

    /**
     * @brief Removes all transitions going to a specific state.
     *
     * @param state The State object.
     *
     * @return The number of transitions removed.
     */
    int removeTransitionsTo(State *state);

    /**
     * @brief Gets the name of the FSM.
     *
     * @return The name of the FSM.
     */
    QString getName();

    /**
     * @brief Gets the comment associated with the FSM.
     *
     * @return The comment associated with the FSM.
     */
    QString getComment();

    /**
     * @brief Gets the initial state of the FSM.
     *
     * @return A pointer to the initial State object.
     */
    State *getInitialState();

    /**
     * @brief Gets a state by its name.
     *
     * @param state_name The name of the state.
     *
     * @return A pointer to the State object, or nullptr if not found.
     */
    State *getState(QString state_name);

    /**
     * @brief Gets a variable by its name.
     *
     * @param variable_name The name of the variable.
     *
     * @return A pointer to the Variable object, or nullptr if not found.
     */
    Variable *getVariable(QString variable_name);

    /**
     * @brief Gets the set of input names.
     *
     * @return A QSet containing the input names.
     */
    QSet<QString> getInputs();

    /**
     * @brief Gets the set of output names.
     *
     * @return A QSet containing the output names.
     */
    QSet<QString> getOutputs();

    /**
     * @brief Gets the map of variables.
     *
     * @return A QMap containing variable names and their corresponding Variable objects.
     */
    QMap<QString, Variable *> getVariables();

    /**
     * @brief Gets the map of states.
     *
     * @return A QMap containing state names and their corresponding State objects.
     */
    QMap<QString, State *> getStates();

    /**
     * @brief Gets all transitions in the FSM.
     *
     * @return A QMultiMap containing all transitions grouped by their source state.
     */
    QMultiMap<QString, Transition *> getTransitions();

    /**
     * @brief Gets all transitions originating from a specific state.
     *
     * @return A QMultiMap containing transitions grouped by their source state.
     */
    QMultiMap<QString, Transition *> getTransitionsFrom();

    /**
     * @brief Gets all transitions targeting a specific state.
     *
     * @return A QMultiMap containing transitions grouped by their target state.
     */
    QMultiMap<QString, Transition *> getTransitionsTo();

    /**
     * @brief Gets all transitions originating from a specific state.
     *
     * @param state A pointer to the source state.
     *
     * @return A QList containing the transitions originating from the specified state.
     */
    QList<Transition *> getTransitionsFrom(State *state);

    /**
     * @brief Gets all transitions targeting a specific state.
     *
     * @param state A pointer to the target state.
     *
     * @return A QList containing the transitions targeting the specified state.
     */
    QList<Transition *> getTransitionsTo(State *state);

    /**
     * @brief Gets all transitions originating from a state by its name.
     *
     * @param state_name The name of the source state.
     *
     * @return A QList containing the transitions originating from the specified state.
     */
    QList<Transition *> getTransitionsFrom(QString state_name);

    /**
     * @brief Gets all transitions targeting a state by its name.
     *
     * @param state_name The name of the target state.
     *
     * @return A QList containing the transitions targeting the specified state.
     */
    QList<Transition *> getTransitionsTo(QString state_name);

    /**
     * @brief Gets the initial XML representation of the FSM.
     *
     * @return The initial XML representation of the FSM.
     */
    QString getInitialFSMXML();

    /**
     * @brief Sets the name of the FSM.
     *
     * @param new_name The new name of the FSM.
     */
    void setName(QString new_name);

    /**
     * @brief Sets the comment associated with the FSM.
     *
     * @param new_comment The new comment for the FSM.
     */
    void setComment(QString new_comment);

    /**
     * @brief Sets the initial state of the FSM.
     *
     * @param new_initial_state A pointer to the new initial State object.
     */
    void setInitialState(State *new_initial_state);

    /**
     * @brief Sets the initial XML representation of the FSM.
     *
     * @param new_initial_fsm_xml The new initial XML representation.
     */
    void setInitialFSMXML(QString initial_fsm_xml);

    /**
     * @brief Pretty prints the FSM structure to the console.
     */
    void prettyPrint();
};
