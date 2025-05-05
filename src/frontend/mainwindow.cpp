#include "mainwindow.hpp"
#include "AutomatView.hpp"
#include "src/backend/xmlparser.hpp"
#include "src/frontend/StateItem.hpp"
#include "src/frontend/TransitionItem.hpp"
#include "src/frontend/ui_mainwindow.h"
#include <qdialogbuttonbox.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include <qlistwidget.h>
#include <qradiobutton.h>
#include <qtextedit.h>
#include <qvector.h>
#include <QLineEdit>
#include <qgraphicsitem.h>
#include <QListWidget>
#include <QDebug>
#include <QFileDialog>

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

  automatView->setGeometry(360, 30, 1551, 561);
  fsm = new FSM("Default FSM");
  // setCentralWidget(automatView);
  connect(automatView, &AutomatView::stateSelected, this,
          &MainWindow::updateStateInfo);
  connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this,
          &MainWindow::saveState);
  connect(automatView, &AutomatView::addState, this, &MainWindow::addState);
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
        QListWidget *widget =
            ui->groupBox->findChild<QListWidget *>("listWidget");
        widget->clear();
        widget->addItem(label);
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

void MainWindow::saveFSM() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save FSM"), "",
                                                  tr("XML Files (*.xml)"));
  if (!fileName.isEmpty()) {
    XMLParser::FSMtoXML(*fsm, fileName);
  }
}