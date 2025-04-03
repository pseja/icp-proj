#pragma once

#include <QStateMachine>

/**
 * @brief Creates a mock state machine implementing the TOF5s automaton
 * 
 * This function creates a QStateMachine that implements the Timer to Off automaton
 * as specified in the project requirements, with proper states, transitions, and properties.
 * 
 * @return A pointer to the created QStateMachine instance
 */
QStateMachine* createTOF5sMachine();
