/**
 * MockStateMachine.cpp
 * 
 * Creates a mock QStateMachine that matches the TOF5s automaton described in the
 * project requirements. This file demonstrates how to construct a state machine
 * that works with the CodeGen class and can be used for testing code generation.
 */

#include "CodeGen.hpp"
#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>

/**
 * Creates a state machine that implements the TOF5s automaton based on the XML specification
 */
QStateMachine* createTOF5sMachine()
{
    // Create the state machine and set its properties
    QStateMachine* machine = new QStateMachine();
    machine->setObjectName("TOF5s");
    machine->setProperty("description", "Timer to off, jednoduchá verze.");
    machine->setProperty("inputs", "in");
    machine->setProperty("outputs", "out");
    machine->setProperty("variables", "int timeout = 5000;");

    // Create states
    QState* idleState = new QState(machine);
    QState* activeState = new QState(machine);
    QState* timingState = new QState(machine);
    
    // Set state names
    idleState->setObjectName("IDLE");
    activeState->setObjectName("ACTIVE");
    timingState->setObjectName("TIMING");
    
    // Set state entry actions - change from camelCase to snake_case
    idleState->setProperty("on_entry_code", "output(\"out\", 0);");
    activeState->setProperty("on_entry_code", "output(\"out\", 1);");
    // TIMING state has no entry action
    
    // Create transitions
    
    // IDLE → ACTIVE: when in=1
    QAbstractTransition* idleToActive = idleState->addTransition(activeState);
    idleToActive->setProperty("condition_code", "defined(\"in\") && atoi_safe(valueof(\"in\")) == 1");
    
    // ACTIVE → TIMING: when in=0
    QAbstractTransition* activeToTiming = activeState->addTransition(timingState);
    activeToTiming->setProperty("condition_code", "defined(\"in\") && atoi_safe(valueof(\"in\")) == 0");
    
    // TIMING → ACTIVE: when in=1
    QAbstractTransition* timingToActive = timingState->addTransition(activeState);
    timingToActive->setProperty("condition_code", "defined(\"in\") && atoi_safe(valueof(\"in\")) == 1");
    
    // TIMING → IDLE: after timeout ms (delayed transition)
    QAbstractTransition* timingToIdle = timingState->addTransition(idleState);
    timingToIdle->setProperty("is_delayed_transition", true);
    timingToIdle->setProperty("delay_ms", "timeout");

    // Add debug output to verify properties are set correctly
    qDebug() << "Created delayed transition: is_delayed_transition=" 
             << timingToIdle->property("is_delayed_transition").toBool()
             << ", delay_ms=" << timingToIdle->property("delay_ms").toString();
    
    // Set the initial state
    machine->setInitialState(idleState);
    
    return machine;
}