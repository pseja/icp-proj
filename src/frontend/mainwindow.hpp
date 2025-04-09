#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <qobjectdefs.h>
#include <qvector.h>
#include "AutomatView.hpp"
#include "src/frontend/StateItem.hpp"
#include "src/backend/fsm.hpp"

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


private:
  FSM *fsm;
  Ui::MainWindow *ui;
  AutomatView *automatView;
  StateItem *selectedState = nullptr;
  void addState(StateItem *state);
  void loadAutomat(const QVector<StateItem *> &states);
};
#endif // MAINWINDOW_HPP
