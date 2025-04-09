/**
 * @file state.hpp
 * @brief Defines the State class, which represents a state in a state machine.
 *
 * This file contains the declaration of the State class, which extends QState
 * and provides additional functionality for managing state-specific attributes
 * such as name, code, and whether the state is an initial state.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#pragma once

#include <QState>

/**
 * @class State
 * @brief Represents a state in a state machine.
 *
 * The State class extends QState and provides additional attributes and methods
 * to manage state-specific properties such as the state's name, associated code,
 * and whether it is the initial state of the state machine.
 */
class State : public QState
{
    Q_OBJECT

  private:
    QString name;            ///< The name of the state.
    QString code;            ///< The code associated with the state.
    bool is_initial = false; ///< Indicates whether this state is the initial state.

  public:
    /**
     * @brief Default constructor.
     * @param parent The parent QState, if any.
     */
    State(QState *parent = nullptr);

    /**
     * @brief Constructor with a name.
     * @param name The name of the state.
     */
    State(QString name);

    /**
     * @brief Gets the name of the state.
     * @return The name of the state.
     */
    QString getName();

    /**
     * @brief Gets the code associated with the state.
     * @return The code associated with the state.
     */
    QString getCode();

    /**
     * @brief Checks if the state is the initial state.
     * @return True if the state is the initial state, false otherwise.
     */
    bool isInitial();

    /**
     * @brief Sets the name of the state.
     * @param new_name The new name of the state.
     */
    void setName(QString new_name);

    /**
     * @brief Sets the code associated with the state.
     * @param new_code The new code to associate with the state.
     */
    void setCode(QString new_code);

    /**
     * @brief Sets whether the state is the initial state.
     * @param new_is_initial True if the state is the initial state, false otherwise.
     */
    void setInitial(bool new_is_initial);
};