/**
 * MockStateMachine.cpp
 * 
 * Creates a mock FSM that matches the TOF5s automaton described in the
 * project requirements. This file demonstrates how to construct a state machine
 * that works with the CodeGen class and can be used for testing code generation.
 */

#include "MockStateMachine.hpp"
#include "fsm.hpp"
#include "state.hpp"
#include "transition.hpp"
#include "variable.hpp"
#include <QDebug>

/**
 * Creates a state machine that implements the TOF5s automaton based on the XML specification
 */
FSM* createTOF5sMachine()
{
    // Create the FSM with name and description
    FSM* fsm = new FSM("TOF5s", "Timer to off, jednoduchá verze.");
    
    // Add inputs and outputs
    fsm->addInput("in");
    fsm->addOutput("out");
    
    // Add variables
    fsm->addVariable("int", "timeout", "5000");

    // Create states with names and entry code
    State* idle = new State("IDLE");
    idle->setCode("output(\"out\", 0);");
    idle->setInitial(true);

    State* active = new State("ACTIVE");
    active->setCode("output(\"out\", 1);");

    State* timing = new State("TIMING"); 
    // TIMING state has no entry action
    
    // Add states to the FSM
    fsm->addState(idle);
    fsm->addState(active);
    fsm->addState(timing);
    fsm->setInitialState(idle);
    
    // Add transitions
    // IDLE → ACTIVE: when in=1
    fsm->addTransition(idle, active, "defined(\"in\") && atoi_safe(valueof(\"in\")) == 1");
    
    // ACTIVE → TIMING: when in=0
    fsm->addTransition(active, timing, "defined(\"in\") && atoi_safe(valueof(\"in\")) == 0");
    
    // TIMING → ACTIVE: when in=1
    fsm->addTransition(timing, active, "defined(\"in\") && atoi_safe(valueof(\"in\")) == 1");
    
    // TIMING → IDLE: after timeout ms (delayed transition)
    // Using the timeout variable value (5000) directly since we can't pass a string
    fsm->addTransition(timing, idle, "", 5000);

    // Add debug output to verify transitions are set correctly
    qDebug() << "Created delayed transition with timeout: 5000ms";
    
    return fsm;
}