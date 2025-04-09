/**
 * @file transition.hpp
 * @brief Defines the Transition class, which represents a transition between states in a state machine.
 *
 * This file contains the declaration of the Transition class, which provides functionality
 * for managing transitions between states, including conditions, delays, and source/target states.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#pragma once

#include "state.hpp"
#include <functional> // for std::function

/**
 * @class Transition
 * @brief Represents a transition between states in a state machine.
 *
 * The Transition class provides attributes and methods to manage transitions between states,
 * including the source state, target state, transition condition, and optional delay.
 */
class Transition : public QObject
{
    Q_OBJECT

  private:
    State *from;       ///< The source state of the transition.
    State *to;         ///< The target state of the transition.
    QString condition; ///< The condition that triggers the transition.
    int delay = -1;    ///< The delay (in milliseconds) before the transition occurs. Default is -1 (no delay).
    bool is_delayed_transition = false; ///< Indicates whether the transition is delayed.

  public:
    /**
     * @brief Constructs a Transition object.
     * @param from The source state of the transition.
     * @param to The target state of the transition.
     * @param condition The condition that triggers the transition.
     * @param delay The delay (in milliseconds) before the transition occurs. Default is -1 (no delay).
     */
    Transition(State *from, State *to, QString condition, int delay = -1);

    /**
     * @brief Gets the source state of the transition.
     * @return A pointer to the source state.
     */
    State *getFrom();

    /**
     * @brief Gets the target state of the transition.
     * @return A pointer to the target state.
     */
    State *getTo();

    /**
     * @brief Gets the condition that triggers the transition.
     * @return The condition as a QString.
     */
    QString getCondition();

    /**
     * @brief Gets the delay before the transition occurs.
     * @return The delay in milliseconds. Returns -1 if no delay is set.
     */
    int getDelay();

    /**
     * @brief Checks if the transition is delayed.
     * @return True if the transition is delayed, false otherwise.
     */
    bool isDelayedTransition() const;

    /**
     * @brief Sets the source state of the transition.
     * @param new_from A pointer to the new source state.
     */
    void setFrom(State *new_from);

    /**
     * @brief Sets the target state of the transition.
     * @param new_to A pointer to the new target state.
     */
    void setTo(State *new_to);

    /**
     * @brief Sets the condition that triggers the transition.
     * @param new_condition The new condition as a QString.
     */
    void setCondition(QString new_condition);

    /**
     * @brief Sets the delay before the transition occurs.
     * @param new_delay The new delay in milliseconds.
     */
    void setDelay(int new_delay);

    /**
     * @brief Sets whether the transition is delayed.
     * @param new_is_delayed_transition True if the transition is delayed, false otherwise.
     */
    void setDelayedTransition(bool new_is_delayed_transition);
};
