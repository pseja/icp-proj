#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <qlistwidget.h>
#include <qobjectdefs.h>
#include <qvector.h>
#include "AutomatView.hpp"
#include "StateItem.hpp"
#include "backend/GuiClient.hpp"
#include "backend/fsm.hpp"

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
    void on_addStateButton_clicked();
    void stateChanged(QString stateName);
    void printoutput(const QString &name, const QString &value);
    void timerstart(const QString &from, const QString &to, const QString &ms);
    void timerend(const QString &from, const QString &to);
    void printmsg(const QString &msg);
    void printerr(const QString &msg, const QString &code);
    void printlog(const QString &msg);
    void onConsoleEnter();
    void showFSMInfo();
    void resizeCode();
public slots:
  void handleStateDeleted();
  void addState(StateItem *state);
  void addTransition(TransitionItem *transition);
  void saveFSM();
  void loadFSM();
  void editTransition(QListWidgetItem *item);
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
};
#endif // MAINWINDOW_HPP
