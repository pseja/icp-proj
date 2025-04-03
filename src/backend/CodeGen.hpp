#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QString>
#include <QAbstractTransition>

/**
 * @class CodeGen
 * @brief Generates C++ code from a Qt state machine model
 * 
 * The CodeGen class transforms a QStateMachine into a standalone C++ application
 * that can execute the state machine logic using Qt's built-in state machine framework.
 */
class CodeGen : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent The parent QObject (default: nullptr)
     */
    explicit CodeGen(QObject *parent = nullptr);

    /**
     * @brief Generates complete C++ code from a state machine
     * @param machine The state machine to generate code from
     * @return Generated C++ code as a QString
     */
    QString generateCode(QStateMachine *machine);

private:
    /**
     * @brief Generates standard header includes
     * @return Code section as QString
     */
    QString generateHeaders();
    
    /**
     * @brief Generates helper functions for FSM operations
     * @return Code section as QString
     */
    QString generateHelperFunctions();
    
    /**
     * @brief Generates global variable declarations
     * @param machine State machine containing property definitions
     * @return Code section as QString
     */
    QString generateVariableDeclarations(QStateMachine *machine);
    
    /**
     * @brief Generates monitoring and debugging functions
     * @return Code section as QString
     */
    QString generateRuntimeMonitoring();
    
    /**
     * @brief Generates state enum and string conversion
     * @param states List of states to include in the enum
     * @return Code section as QString
     */
    QString generateStateEnum(const QList<QState*>& states);
    
    /**
     * @brief Generates a main function that uses QStateMachine
     * @param machine State machine containing the initial state
     * @param states List of all states in the machine
     * @return Code section as QString
     */
    QString generateQStateMachineMain(QStateMachine *machine, const QList<QState*>& states);
};

#endif // CODEGEN_HPP