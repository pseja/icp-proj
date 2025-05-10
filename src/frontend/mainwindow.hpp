#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <qlistwidget.h>
#include <qobjectdefs.h>
#include <qprocess.h>
#include <qvector.h>
#include "AutomatView.hpp"
#include "StateItem.hpp"
#include "backend/GuiClient.hpp"
#include "backend/fsm.hpp"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    

private slots:
    void saveState();
    void updateStateInfo(StateItem *state);
    void updateTransitionInfo(TransitionItem *transition);
    void on_addStateButton_clicked();
    void stateChanged(QString stateName);
    void printoutput(const QString &name, const QString &value);
    void timerstart(const QString &from, const QString &to, const QString &ms);
    void timerend(const QString &from, const QString &to);
    void printmsg(const QString &msg);
    void printerr(const QString &msg, const QString &code);
    void printlog(const QString &msg);
    void requestedFSM(const QString &model);
    void onConsoleEnter();
    void showFSMInfo();
    void resizeCode();
    void clearFSM();
    void refreshFSM();
    void sudoclearFSM();
    void deleteVar();
    void stopFSM();
    void connectToFSM();

public slots:
  void handleStateDeleted(StateItem *state);
  void addState(StateItem *state);
  void addTransition(TransitionItem *transition);
  void saveFSM();
  void loadFSM();
  //void editTransition(QListWidgetItem *item);
  void saveTransition();
  void runFSM();
  void saveVars();
  //void onReadyRead(const QString &data);

private:
  FSM *fsm;
  Ui::MainWindow *ui;
  AutomatView *automatView;
  StateItem *selectedState = nullptr;
  void loadAutomat(const QVector<StateItem *> &states);
  QVector<TransitionItem*> transitionItemsForSelectedState;
  TransitionItem *selectedTransition = nullptr;
  GuiClient *client;
  QProcess *serverProcess = nullptr;

QString helpText = R"(Writing code for state actions and transition conditions
========================================================

Enter C++ code to define what happens when a state is entered, or to specify conditions for transitions. 
Here’s what you need to know:

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
#endif // MAINWINDOW_HPP
