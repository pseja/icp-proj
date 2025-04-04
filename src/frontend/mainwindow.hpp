#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <qvector.h>
#include "AutomatView.hpp"
#include "src/frontend/StateItem.hpp"

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
    void updateStateInfo(StateItem *state);
    void on_addStateButton_clicked();

private:
  Ui::MainWindow *ui;
  AutomatView *automatView;
  void addState(StateItem *state);
  void loadAutomat(const QVector<StateItem *> &states);
};
#endif // MAINWINDOW_HPP
