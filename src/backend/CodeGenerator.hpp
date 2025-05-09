/**
 * @file CodeGenerator.hpp
 * @brief Header file for the CodeGenerator class for generating C++ code from FSM memory instances.
 * @author xcsirim00
 * @date 8. May 2025
 */

#pragma once

#include <QObject>
#include <QString>

// Forward declarations
class FSM;
class State;
class Transition;
class Variable;

/**
 * @brief The CodeGenerator class generates C++ code from FSM instances
 */
class CodeGenerator : public QObject {
    Q_OBJECT

   public:
    explicit CodeGenerator(QObject *parent = nullptr);

    /**
     * @brief Generates C++ code from a state machine
     * @param fsm The state machine to generate code from
     * @return Generated C++ code
     */
    QString generateCode(FSM *fsm);

   private:
    QString generateHeaders();
    QString generateVariableDeclarations(FSM *fsm);
    QString generateRuntimeMonitoring();
    QString generateHelperFunctions(FSM *fsm);
    QString generateMainFunction(FSM *fsm);
    QString generateQStateMachineMain(FSM *fsm);
    QString generateTcpXmlProtocolServer(FSM *fsm);
    QString generateStateSetup(FSM *fsm);
    QString generateTerminalInputHandler(FSM *fsm);
    QString generateInputEventClass();
    QString generateGeneratedTransitionClass();

    /**
     * @brief Generate transition code based on a transition
     * @param transition The transition to generate code for
     * @param sourceState The source state
     * @param targetState The target state
     * @return Generated C++ code for the transition
     */
    QString generateTransitionCode(Transition *transition, const State *sourceState, const State *targetState);
};
