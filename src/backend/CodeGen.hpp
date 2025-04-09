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
 * @brief The StructuredTransition class represents a transition triplet
 * 
 * A transition triplet has the format: "input_event_name [ boolean_expression ] @ delay_in_ms"
 * where each part is optional.
 */
class StructuredTransition {
public:
    QString trigger;    ///< Input event name (optional)
    QString condition;  ///< Boolean expression (optional)
    QString delay;      ///< Delay expression in ms (optional)

    bool hasTrigger() const { return !trigger.isEmpty(); }
    bool hasCondition() const { return !condition.isEmpty(); }
    bool hasDelay() const { return !delay.isEmpty(); }
    
    QString toString() const;
    static StructuredTransition fromString(const QString &tripletStr);
    static StructuredTransition fromTransition(Transition* transition);
};

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

    /**
     * @brief Convert a Transition object to a StructuredTransition representation
     * @param transition The Transition object
     * @return A StructuredTransition representation
     */
    StructuredTransition structuredTransitionFromTransition(Transition *transition);
};

#endif // CODEGEN_HPP