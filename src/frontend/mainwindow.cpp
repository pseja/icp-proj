#include "mainwindow.hpp"
#include "AutomatView.hpp"
#include "src/frontend/StateItem.hpp"
#include "src/frontend/ui_mainwindow.h"
#include <qdialogbuttonbox.h>
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
  connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this, &MainWindow::saveState);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_addStateButton_clicked() {

}

void MainWindow::saveState() {
  if (selectedState) {
    selectedState->setName(ui->lineEdit->text());
    selectedState->setCodeSegment(ui->textEdit->toPlainText());
  }
}

//aka clicking on state calls this with pointer to that state
void MainWindow::updateStateInfo(StateItem *state) {
  selectedState = state;
  ui->groupBox->setEnabled(true);
  QLineEdit *lineEdit = ui->groupBox->findChild<QLineEdit *>("lineEdit");
  QTextEdit *textedit = ui->groupBox->findChild<QTextEdit *>("textEdit");
  if (lineEdit) {
    lineEdit->setText(selectedState->getName());
  }
  if (textedit) {
    textedit->setText(selectedState->getCodeSegment());
  }
}

void MainWindow::loadAutomat(const QVector<StateItem *> &states) {
  automatView->clearMask();
  for (StateItem *state : states) {
    //automatView->addState(state);
  }
}