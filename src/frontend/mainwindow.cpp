#include "mainwindow.hpp"
#include "AutomatView.hpp"
#include "src/frontend/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  automatView = new AutomatView(this);
  //automatView->setMinimumSize(400, 300);
  
  //setCentralWidget(automatView);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_addStateButton_clicked() {
  automatView->addState(100, 100);
}
