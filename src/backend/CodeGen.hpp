#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <QObject>
#include <QString>

// Forward declarations
class FSM;
class State;
class Transition;
class Variable;

/**
 * @brief The CodeGen class generates C++ code from FSM instances
 */
class CodeGen : public QObject
{
    Q_OBJECT

public:
    explicit CodeGen(QObject *parent = nullptr);

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
    QString generateHelperFunctions();
    QString generateQStateMachineMain(FSM *fsm);
    
    /**
     * @brief Generate transition code based on a transition
     * @param transition The transition to generate code for
     * @param sourceState The source state
     * @param targetState The target state
     * @return Generated C++ code for the transition
     */
    QString generateTransitionCode(Transition *transition,
                                const State *sourceState, 
                                const State *targetState);
};

#endif // CODEGEN_HPP