/**
 * @file mainwindow.hpp
 * @brief Defines the MainWindow class and it's methods which is the primary GUI for the FSM editor.
 *
 * @author Vaclav Sovak (xsovakv00)
 * @date 11-05-2025
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <qlistwidget.h>
#include <qmap.h>
#include <qobjectdefs.h>
#include <qprocess.h>
#include <qvector.h>

#include <QHeaderView>
#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QVBoxLayout>

#include "AutomatView.hpp"
#include "StateItem.hpp"
#include "backend/GuiClient.hpp"
#include "backend/fsm.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main window of the FSM editor.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    /**
     * @brief Constructs the main window.
     *
     * @param parent Optional parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the main window.
     */
    ~MainWindow();

   private slots:
    /**
     * @brief Saves the currently selected state's properties.
     */
    void saveState();

    /**
     * @brief Updates the state property panel when a state is selected.
     *
     * @param state The selected StateItem.
     */
    void updateStateInfo(StateItem *state);

    /**
     * @brief Updates the transition property panel when a transition is selected.
     *
     * @param transition The selected TransitionItem.
     */
    void updateTransitionInfo(TransitionItem *transition);

    /**
     * @brief Handles the Add State button click event.
     */
    void on_addStateButton_clicked();

    /**
     * @brief Handles state change events from the simulator.
     *
     * @param stateName Name of the state that was entered.
     */
    void stateChanged(QString stateName);

    /**
     * @brief Displays an output value in the GUI console.
     *
     * @param name Name of the output.
     * @param value Value of the output.
     */
    void printoutput(const QString &name, const QString &value);

    /**
     * @brief Displays an input value in the GUI console.
     *
     * @param name Name of the input.
     * @param value Value of the input.
     */
    void printinput(const QString &name, const QString &value);

    /**
     * @brief Displays a variable value in the GUI console.
     *
     * @param name Name of the variable.
     * @param value Value of the variable.
     */
    void printvariable(const QString &name, const QString &value);

    /**
     * @brief Logs the start of a timer in the simulation.
     *
     * @param from Source state.
     * @param to Target state.
     * @param ms Timer duration in milliseconds.
     */
    void timerstart(const QString &from, const QString &to, const QString &ms);

    /**
     * @brief Logs the end of a timer in the simulation.
     *
     * @param from Source state.
     * @param to Target state.
     */
    void timerend(const QString &from, const QString &to);

    /**
     * @brief Displays a message in the GUI console.
     *
     * @param msg The message to display.
     */
    void printmsg(const QString &msg);

    /**
     * @brief Displays an error message in the GUI console.
     *
     * @param msg Error message.
     * @param code Error code or additional information.
     */
    void printerr(const QString &msg, const QString &code);

    /**
     * @brief Logs general information in the GUI console.
     *
     * @param msg The log message.
     */
    void printlog(const QString &msg);

    /**
     * @brief Handles receiving the requested FSM from the server.
     *
     * @param model XML representation of the FSM.
     */
    void requestedFSM(const QString &model);

    /**
     * @brief Updates UI based on the FSM status.
     *
     * @param status Current status of the FSM.
     */
    void fsmStatus(const FsmStatus &status);

    /**
     * @brief Handles the user pressing Enter in the console input.
     */
    void onConsoleEnter();

    /**
     * @brief Displays information about the current FSM.
     */
    void showFSMInfo();

    /**
     * @brief Resizes the code editor to fit the content.
     */
    void resizeCode();

    /**
     * @brief Clears the current FSM.
     */
    void clearFSM();

    /**
     * @brief Refreshes the FSM view.
     */
    void refreshFSM();

    /**
     * @brief Forces a clean reload of the FSM.
     */
    void sudoclearFSM();

    /**
     * @brief Deletes the selected variable.
     */
    void deleteVar();

    /**
     * @brief Stops the running FSM simulation.
     */
    void stopFSM();

    /**
     * @brief Connects to the FSM server for simulation.
     */
    void connectToFSM();

    /**
     * @brief Exports the current FSM to XML format.
     */
    void exportXML();

    /**
     * @brief Exports the current FSM to C++ code.
     */
    void exportCPP();

   public slots:
    /**
     * @brief Handles state deletion events.
     *
     * @param state The StateItem being deleted.
     */
    void handleStateDeleted(StateItem *state);

    /**
     * @brief Handles transition deletion events.
     *
     * @param transition The TransitionItem being deleted.
     */
    void handleTransitionDeleted(TransitionItem *transition);

    /**
     * @brief Adds a new state to the FSM.
     *
     * @param state The StateItem to add.
     */
    void addState(StateItem *state);

    /**
     * @brief Adds a new transition to the FSM.
     *
     * @param transition The TransitionItem to add.
     */
    void addTransition(TransitionItem *transition);

    /**
     * @brief Saves the current FSM to a file.
     */
    void saveFSM();

    /**
     * @brief Loads an FSM from a file.
     */
    void loadFSM();

    /**
     * @brief Saves changes to the currently selected transition.
     */
    void saveTransition();

    /**
     * @brief Runs the FSM simulation.
     */
    void runFSM();

    /**
     * @brief Saves variables defined in the FSM.
     */
    void saveVars();

    /**
     * @brief Demonstrates sample functionality of the FSM.
     */
    void tryMe();

    /**
     * @brief Shows code guidelines for writing state actions and transition conditions.
     */
    void codeGuidelines();

    /**
     * @brief Shows information about the application authors.
     */
    void authors();

   private:
    /**
     * @brief The FSM this window is editing.
     */
    FSM *fsm;

    /**
     * @brief UI components of the main window.
     */
    Ui::MainWindow *ui;

    /**
     * @brief The visual editor for the FSM.
     */
    AutomatView *automatView;

    /**
     * @brief Currently selected state in the editor.
     */
    StateItem *selectedState = nullptr;

    /**
     * @brief Loads automaton with the specified states.
     *
     * @param states Vector of StateItems to load.
     */
    void loadAutomat(const QVector<StateItem *> &states);

    /**
     * @brief Cleans up temporary files created during execution.
     */
    void cleanupTempFiles();

    /**
     * @brief List of transitions connected to the currently selected state.
     */
    QVector<TransitionItem *> transitionItemsForSelectedState;

    /**
     * @brief Currently selected transition in the editor.
     */
    TransitionItem *selectedTransition = nullptr;

    /**
     * @brief Client for communicating with the FSM server.
     */
    GuiClient *client;

    /**
     * @brief Process for running the FSM server.
     */
    QProcess *serverProcess = nullptr;

    /**
     * @brief Map of input names to their values.
     */
    QMap<QString, QString> inputs;

    /**
     * @brief Map of output names to their values.
     */
    QMap<QString, QString> outputs;

    /**
     * @brief Flag indicating whether auto-connect has been attempted.
     */
    bool autoConnectAttempted = false;

    /**
     * @brief Help text for writing code in the editor.
     */
    QString helpText = R"(Writing code for state actions and transition conditions
========================================================

Enter C++ code to define what happens when a state is entered, or to specify conditions for transitions. 
Here's what you need to know:

Accessing variables
-------------------
- Custom variables: Use them by name (e.g., counter = 0;).
- Inputs: Use valueof("inputName") to get the value as a QString.
- Check if input is set: defined("inputName") returns true if the input has a value.
- Check if input was called as an event: called("inputName") returns true if triggered (and resets the flag).

Note: Values are handled as QStrings. Use the provided helper function for conversions:
- Use `Qtoi(qstring)` to convert a QString to int.

Setting outputs
---------------
- Use output("outputName", value); to set an output (value can be a number or string).

Timers and time
---------------
- Use elapsed() to get milliseconds since the state was entered.

Writing conditions (for transitions)
------------------------------------
- Write any valid C++ boolean expression.
- You can use variables, valueof(), defined(), and called() in your expressions.

Limitations
-----------
- All variables are global; avoid name conflicts.
- No advanced C++ features (e.g., custom includes, classes).
- Code runs synchronously; avoid long-running operations.

Example snippets
----------------
- Set a variable and output:
  counter = counter + 1;
  output("led", counter > 5 ? 1 : 0);
- Transition condition:
  valueof("button") == "pressed" && counter < 10
- Convert input to int:
  Qtoi(valueof("input1")) > 5

Tip: Keep your code simple and focused on logic. Use the provided helper functions for interacting with inputs, outputs, and logging. 
Remember to use Qtoi for conversions when working with QStrings. Also check out official QString Qt5.9 documentation. )";
};
#endif  // MAINWINDOW_HPP
