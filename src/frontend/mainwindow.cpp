#include "mainwindow.hpp"
#include "AutomatView.hpp"
#include "src/frontend/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setFixedSize(1920, 1080);
  resize(1920, 1080);
  setMinimumSize(800, 600);
  //automatView = new AutomatView(this);
  //automatView->setMinimumSize(400, 300);
  
  //setCentralWidget(automatView);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_addStateButton_clicked() {

}
