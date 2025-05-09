/**
 * @file state.hpp
 * @brief Defines the State class and it's methods, which represent a state in the FSM.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#pragma once

#include <QState>

/**
 * @class State
 *
 * @brief Represents a state in a FSM.
 */
class State : public QState {
    Q_OBJECT

   private:
    QString name;             ///< The name of the state.
    QString code;             ///< The code associated with the state.
    bool is_initial = false;  ///< Flag if this state is the initial state.

   public:
    /**
     * @brief Default constructor.
     *
     * @param parent The parent QState, if any.
     */
    State(QState *parent = nullptr);

    /**
     * @brief Constructor with a name.
     *
     * @param name The name of the state.
     */
    State(QString name);

    /**
     * @brief Gets the name of the state.
     *
     * @return The name of the state.
     */
    QString getName();

    /**
     * @brief Gets the code associated with the state.
     *
     * @return The code associated with the state.
     */
    QString getCode();

    /**
     * @brief Checks if the state is the initial state.
     *
     * @return True if the state is the initial state, false otherwise.
     */
    bool isInitial();

    /**
     * @brief Sets the name of the state.
     *
     * @param new_name The new name of the state.
     */
    void setName(QString new_name);

    /**
     * @brief Sets the code associated with the state.
     *
     * @param new_code The new code to associate with the state.
     */
    void setCode(QString new_code);

    /**
     * @brief Sets whether the state is the initial state.
     *
     * @param new_is_initial True if the state is the initial state, false otherwise.
     */
    void setInitial(bool new_is_initial);
};