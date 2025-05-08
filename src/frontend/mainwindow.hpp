#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <qlistwidget.h>
#include <qobjectdefs.h>
#include <qvector.h>
#include "AutomatView.hpp"
#include "StateItem.hpp"
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

private:
  FSM *fsm;
  Ui::MainWindow *ui;
  AutomatView *automatView;
  StateItem *selectedState = nullptr;
  void loadAutomat(const QVector<StateItem *> &states);
  QVector<TransitionItem*> transitionItemsForSelectedState;
  TransitionItem* selectedTransition = nullptr;
};
#endif // MAINWINDOW_HPP
