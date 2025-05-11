/**
 * @file CodeGenerator.hpp
 * @brief Header file for the CodeGenerator class for generating C++ code from FSM memory instances.
 *
 * @author xcsirim00
 * @date 8. May 2025
 */

#pragma once

#include <QObject>
#include <QString>

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
    /**
     * @brief Constructor for CodeGenerator.
     *
     * @param parent Optional parent QObject.
     */
    explicit CodeGenerator(QObject *parent = nullptr);

    /**
     * @brief Generate the full C++ code for a given FSM.
     *
     * @param fsm Pointer to the FSM object to generate code from.
     * @return The generated C++ code as a QString.
     */
    QString generateCode(FSM *fsm);

   private:
    /**
     * @brief Generate standard C++ header includes for the generated file.
     *
     * @return C++ code section with all required #include directives as a QString.
     */
    QString generateHeaders();

    /**
     * @brief Generate helper and utility functions for the generated FSM code.
     *
     * Includes helpers for input/output, value access, conversion, event flagging,
     * timer management, and status reporting.
     *
     * @param fsm Pointer to the FSM object for which helpers are generated.
     * @return C++ code section with helper functions as a QString.
     */
    QString generateHelperFunctions(FSM *fsm);

    /**
     * @brief Generate global declarations for standard and custom variables required for the generated FSM code.
     *
     * @param fsm Pointer to the FSM object containing variable definitions.
     * @return C++ code section with variable declarations as a QString.
     */
    QString generateVariableDeclarations(FSM *fsm);

    /**
     * @brief Generate runtime monitoring functions for the FSM.
     *
     * Also provides logging, debug, and help display utilities for state transitions,
     * events, and debugging.
     *
     * @return C++ code section with runtime monitoring functions as a QString.
     */
    QString generateRuntimeMonitoring();

    /**
     * @brief Generate C++ code for a single FSM transition.
     *
     * @param transition Pointer to the transition object.
     * @param sourceState Pointer to the source state.
     * @param targetState Pointer to the target state.
     * @return C++ code for the transition as a QString.
     */
    QString generateTransitionCode(Transition *transition, const State *sourceState, const State *targetState);

    /**
     * @brief Generate the main function and core classes for the FSM application.
     *
     * Additionally state setup, transitions, and event an loop.
     *
     * @param fsm Pointer to the FSM object containing states and transitions.
     * @return C++ code section with the main function and related classes as a
     * QString.
     */
    QString generateMainFunction(FSM *fsm);

    /**
     * @brief Generate the TCP XML protocol server logic for the FSM.
     *
     * @param fsm Pointer to the FSM object.
     * @return C++ code section with TCP server logic as a QString.
     */
    QString generateTcpXmlProtocolServer(FSM *fsm);

    /**
     * @brief Generate the terminal input handler logic for the FSM.
     *
     * @param fsm Pointer to the FSM object.
     * @return C++ code section with terminal input handler as a QString.
     */
    QString generateTerminalInputHandler(FSM *fsm);

    /**
     * @brief Generate the InputEvent class for FSM input events.
     *
     * Defines a custom QEvent subclass for input changes.
     *
     * @return C++ code section with the InputEvent class as a QString.
     */
    QString generateInputEventClass();

    /**
     * @brief Generate the GeneratedTransition class for FSM transitions.
     *
     * Defines a custom QAbstractTransition subclass for runtime transitions in the
     * generated FSM code.
     *
     * @return C++ code section with the GeneratedTransition class as a QString.
     */
    QString generateGeneratedTransitionClass();
};
