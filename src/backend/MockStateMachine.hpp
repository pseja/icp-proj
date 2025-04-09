#pragma once

#include "fsm.hpp"

/**
 * @brief Creates a mock state machine implementing the TOF5s automaton
 * 
 * This function creates an FSM that implements the Timer to Off automaton
 * as specified in the project requirements, with proper states, transitions, and properties.
 * 
 * @return A pointer to the created FSM instance
 */
FSM* createTOF5sMachine();
