#include "mainwindow.hpp"
#include "AutomatView.hpp"
#include "src/backend/state.hpp"
#include "src/backend/transition.hpp"
#include "src/backend/xmlparser.hpp"
#include "src/frontend/StateItem.hpp"
#include "src/frontend/TransitionItem.hpp"
#include "src/frontend/ui_mainwindow.h"
#include "src/backend/variable.hpp"
#include <qdialogbuttonbox.h>
#include <qgraphicsitem.h>
#include <qlineedit.h>
#include <qlist.h>
#include <qlistwidget.h>
#include <qmap.h>
#include <qradiobutton.h>
#include <qtextedit.h>
#include <qvector.h>
#include <QLineEdit>
#include <qgraphicsitem.h>
#include <QListWidget>
#include <QDebug>
#include <QFileDialog>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  resize(1920, 1080);
  setMinimumSize(800, 600);
  fsm = new FSM("Default FSM");
  automatView = new AutomatView(fsm, this);
  automatView->setMinimumSize(400, 300);
  ui->logConsole->setStyleSheet("background-color:rgb(203, 202, 202);");
  ui->lineEdit_2->setStyleSheet("background-color:rgb(160, 160, 160);");
  automatView->setGeometry(710, 30, 1205, 960);
  fsm = new FSM("Default FSM");
  fsm->addVariable(new Variable("int", "dummy", 0));
  // setCentralWidget(automatView);
  
  connect(automatView, &AutomatView::stateSelected, this,&MainWindow::updateStateInfo);
  connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this,&MainWindow::saveState);
  connect(automatView, &AutomatView::addState, this, &MainWindow::addState);
  connect(automatView, &AutomatView::addTransition, this, &MainWindow::addTransition);
  connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFSM);
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::loadFSM);
  connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::editTransition);
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&MainWindow::saveTransition);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_addStateButton_clicked() {

}

void MainWindow::saveState() {
  if (selectedState) {
    // qDebug() << "got pointer, trying to update name";
    // selectedState->setName(ui->lineEdit->text());
    // qDebug() << "trying to update code";
    // selectedState->setCodeSegment(ui->textEdit->toPlainText());
    selectedState->updateState(ui->lineEdit->text(), ui->textEdit->toPlainText(), ui->radioButton_3->isChecked());
  }
}

//aka clicking on state, calls this with pointer to that state
void MainWindow::updateStateInfo(StateItem *state) {
  selectedState = state;
  ui->groupBox->setEnabled(true);
  QLineEdit *lineEdit = ui->groupBox->findChild<QLineEdit *>("lineEdit");
  QTextEdit *textedit = ui->groupBox->findChild<QTextEdit *>("textEdit");
  QRadioButton *radio = ui->groupBox->findChild<QRadioButton *>("radioButton_3");
  if (lineEdit) {
    lineEdit->setText(selectedState->state->getName());
  }
  if (textedit) {
    textedit->setText(selectedState->state->getCode());
  }
  if (radio) {
    radio->setChecked(selectedState->state->isInitial());
  }

  QListWidget *widget = ui->groupBox->findChild<QListWidget *>("listWidget");
  widget->clear();
  QList<QGraphicsItem *> items = automatView->scene()->items();
  for (QGraphicsItem *item : items) {
    qDebug() << "starting for in items: " << item;
    if (typeid(*item) == typeid(TransitionItem)) {
      qDebug() << "found transition item!";
      TransitionItem *trans = dynamic_cast<TransitionItem *>(item);
      if (trans->getFrom() == state || trans->getTo() == state) {
        qDebug() << "found match!";
        QString direction = (trans->getFrom() == state) ? "→" : "←";
        QString label = QString(trans->getFrom()->state->getName() + direction +
                                trans->getTo()->state->getName() + " : " +
                                trans->transition->getEvent());

        widget->addItem(label);
        transitionItemsForSelectedState.append(trans);
      }
    }
  }
}

void MainWindow::handleStateDeleted() {
  ui->lineEdit->clear();
  ui->textEdit->clear();
  ui->listWidget->clear();
  ui->groupBox->setEnabled(false);
  selectedState = nullptr;
}

void MainWindow::loadAutomat(const QVector<StateItem *> &states) {
  automatView->clearMask();
  for (StateItem *state : states) {
    //automatView->addState(state);
  }
}

void MainWindow::addState(StateItem *StateItem) {
  fsm->addState(StateItem->state);
}

void MainWindow::addTransition(TransitionItem *transition) {
  fsm->addTransition(transition->transition);
}

void MainWindow::saveFSM() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save FSM"), "",
                                                  tr("XML Files (*.xml)"));
  for (State *state : fsm->getStates()) {
    qDebug() << "State in FSM:" << state->getName();
  }
  if (!fileName.isEmpty()) {
    XMLParser::FSMtoXML(*fsm, fileName);
  }
}

void MainWindow::editTransition(QListWidgetItem *item) {
  int row = ui->listWidget->row(item);
  if (row < 0 || row >= transitionItemsForSelectedState.size()) return;
  selectedTransition = transitionItemsForSelectedState[row];

  ui->groupBox_2->setEnabled(true);
  QLineEdit *lineEdit = ui->groupBox_2->findChild<QLineEdit *>("eventLineEdit");
  QLineEdit *conditionEdit = ui->groupBox_2->findChild<QLineEdit *>("conditionLineEdit");
  QLineEdit *delayEdit = ui->groupBox_2->findChild<QLineEdit *>("delayLineEdit");
  QLineEdit *delayVarEdit = ui->groupBox_2->findChild<QLineEdit *>("delayVarLineEdit");
  lineEdit->setText(selectedTransition->transition->getEvent());
  conditionEdit->setText(selectedTransition->transition->getCondition());
  delayEdit->setText(QString::number(selectedTransition->transition->getDelay()));
  delayVarEdit->setText(selectedTransition->transition->getDelayVariableName());
}

void MainWindow::saveTransition() {
    if (!selectedTransition) return;

    QLineEdit *lineEdit = ui->groupBox_2->findChild<QLineEdit *>("eventLineEdit");
    QLineEdit *conditionEdit = ui->groupBox_2->findChild<QLineEdit *>("conditionLineEdit");
    QLineEdit *delayEdit = ui->groupBox_2->findChild<QLineEdit *>("delayLineEdit");
    QLineEdit *delayVarEdit = ui->groupBox_2->findChild<QLineEdit *>("delayVarLineEdit");

    selectedTransition->transition->setEvent(lineEdit->text());
    selectedTransition->transition->setCondition(conditionEdit->text());
    selectedTransition->setLabel(conditionEdit->text());
    selectedTransition->transition->setDelay(delayEdit->text().toInt());
    selectedTransition->transition->setDelayVariableName(delayVarEdit->text());
}

void MainWindow::loadFSM() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open FSM"), "",
                                                  tr("XML Files (*.xml)"));
  if (!fileName.isEmpty()) {
    automatView->scene()->clear();
    XMLParser::XMLtoFSM(fileName, *fsm);

    int cols = ceil(sqrt(fsm->getStates().size()));
    int spacing = 120;
    int row = 0, col = 0;
    QMap<State*, StateItem*> state_map;
    for (State *state : fsm->getStates()) {
      StateItem *stateItem = new StateItem(state->getName(), state->getCode());
      stateItem->state = state;
      stateItem->setPos(100 + col * spacing, 100 + row * spacing);
      automatView->scene()->addItem(stateItem);
      state_map[state] = stateItem;
      if (++col >= cols) { col = 0; ++row; }
    }

    QMap<QPair<QString, QString>, int> transition_count;
    for (Transition *transition : fsm->getTransitions()) {
      qDebug() << "Transition from" << transition->getFrom()->getName()
               << "to" << transition->getTo()->getName();
      State *from = transition->getFrom();
      State *to = transition->getTo();
      StateItem *fromItem = state_map.value(from, nullptr);
      StateItem *toItem = state_map.value(to, nullptr);
      if (!fromItem || !toItem) {
        qDebug() << "Pointer mismatch for transition from" << from->getName() << "to" << to->getName();
      }
      if (fromItem && toItem) {
        QPair<QString, QString> pair(from->getName(), to->getName());
        int count = transition_count.value(pair, 0);
        qDebug() << "Transition count for" << pair << ":" << count;
        transition_count[pair] = count + 1;

        TransitionItem *transItem = new TransitionItem(fromItem, toItem, nullptr, count);
        transItem->transition = transition;
        transItem->setLabel(transition->getCondition());
        automatView->scene()->addItem(transItem);
      }
    }
    
  }
}