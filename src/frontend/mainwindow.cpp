#include "mainwindow.hpp"
#include "AutomatView.hpp"
#include "src/frontend/StateItem.hpp"
#include "src/frontend/ui_mainwindow.h"
#include <qdialogbuttonbox.h>
#include <qradiobutton.h>
#include <qtextedit.h>
#include <qvector.h>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  resize(1920, 1080);
  setMinimumSize(800, 600);
  automatView = new AutomatView(this);
  automatView->setMinimumSize(400, 300);
  
  automatView->setGeometry(360, 30, 1551, 561);
  // setCentralWidget(automatView);
  connect(automatView, &AutomatView::stateSelected, this,
          &MainWindow::updateStateInfo);
  connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this,
          &MainWindow::saveState);
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

//aka clicking on state calls this with pointer to that state
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
}

void MainWindow::handleStateDeleted() {
  ui->lineEdit->clear();
  ui->textEdit->clear();
  ui->groupBox->setEnabled(false);
  selectedState = nullptr;
}

void MainWindow::loadAutomat(const QVector<StateItem *> &states) {
  automatView->clearMask();
  for (StateItem *state : states) {
    //automatView->addState(state);
  }
}