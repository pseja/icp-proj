#include "mainwindow.hpp"
#include "AutomatView.hpp"
#include "backend/CodeGenerator.hpp"
#include "backend/GuiClient.hpp"
#include "backend/state.hpp"
#include "backend/transition.hpp"
#include "backend/xmlparser.hpp"
#include "StateItem.hpp"
#include "TransitionItem.hpp"
#include "ui_mainwindow.h"
#include "backend/variable.hpp"
#include <qdialogbuttonbox.h>
#include <qgraphicsitem.h>
#include <qdir.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlist.h>
#include <qlistwidget.h>
#include <qmap.h>
#include <qnamespace.h>
#include <qprocess.h>
#include <qradiobutton.h>
#include <qregularexpression.h>
#include <qtextedit.h>
#include <qvector.h>
#include <QLineEdit>
#include <qgraphicsitem.h>
#include <QListWidget>
#include <QDebug>
#include <QFileDialog>
#include <cmath>
#include <QProcess>
#include <QTimer>
#include <QDateTime>

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
  ui->console->setStyleSheet("background-color:rgb(138, 138, 138);");
  automatView->setGeometry(710, 30, 1205, 960);
  fsm = new FSM("Default FSM");
  fsm->addVariable(new Variable("int", "dummy", 0));
  client = new GuiClient("127.0.0.1", 54323, this);
  ui->groupBox_3->setEnabled(true);
  ui->groupBox_3->setVisible(true);
  ui->groupBox_3->move(0, 0);
  showFSMInfo();
  ui->groupBox->setVisible(false);

  //this will be done later, requires cmake moficiation and new speacial xml file
  //ui->pushButton->setIcon(QIcon(":/icons/11209980.png"));
  //ui->pushButton->setIconSize(QSize(25, 25));
  // setCentralWidget(automatView);
  
  //-------------------------AUTOMAT VIEW SIGNALS-----------------
  connect(automatView, &AutomatView::showFSMInfo, this, &MainWindow::showFSMInfo);
  connect(automatView, &AutomatView::stateSelected, this,&MainWindow::updateStateInfo);
  connect(automatView, &AutomatView::addState, this, &MainWindow::addState);
  connect(automatView, &AutomatView::addTransition, this, &MainWindow::addTransition);
  //-------------------------------------------------------------

  //-------------------------UI SIGNALS--------------------------
  connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFSM);
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::loadFSM);
  connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::editTransition);
  connect(ui->buttonRun, &QPushButton::pressed, this, &MainWindow::runFSM);
  connect(ui->pushButton, &QPushButton::pressed, this, &MainWindow::resizeCode);
  connect(ui->buttonClear, &QPushButton::pressed, this, &MainWindow::clearFSM);
  connect(ui->buttonRefresh, &QPushButton::pressed, this, &MainWindow::refreshFSM);
  connect(ui->console, &QLineEdit::returnPressed, this, &MainWindow::onConsoleEnter);
  connect(ui->buttonBox_3, &QDialogButtonBox::accepted, this, &MainWindow::saveVars);
  connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this,&MainWindow::saveState);
  //--------------------------------------------------------------

  //------------------------CLIENT SIGNALS------------------------
  connect(client, &GuiClient::stateChange, this, &MainWindow::stateChanged);
  connect(client, &GuiClient::printoutput, this, &MainWindow::printoutput);
  connect(client, &GuiClient::timerstart, this, &MainWindow::timerstart);
  connect(client, &GuiClient::timerend, this, &MainWindow::timerend);
  connect(client, &GuiClient::printmsg, this, &MainWindow::printmsg);
  connect(client, &GuiClient::printerr, this, &MainWindow::printerr);
  connect(client, &GuiClient::printlog, this, &MainWindow::printlog);
  //---------------------------------------------------------------
}

MainWindow::~MainWindow() { automatView->disconnect(); client->disconnect(); delete ui; }

void MainWindow::on_addStateButton_clicked() {

}

void MainWindow::saveState() {
  if (selectedState) {
    // qDebug() << "got pointer, trying to update name";
    // selectedState->setName(ui->lineEdit->text());
    // qDebug() << "trying to update code";
    // selectedState->setCodeSegment(ui->textEdit->toPlainText());
    selectedState->updateState(ui->lineEdit->text(), ui->textEdit->toPlainText(), ui->radioButton_3->isChecked());
    saveTransition();
  }
}

void MainWindow::showFSMInfo() {
  ui->groupBox->setEnabled(false);
  ui->groupBox->setVisible(false);
  ui->groupBox_3->setEnabled(true);
  ui->groupBox_3->setVisible(true);

  QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  textEdit->clear();
  for (QString in : fsm->getInputs()) { textEdit->append(in); }

  QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  textEdit1->clear();
  for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

  QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  textEdit2->clear();
  for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName()); }
  
  QLineEdit *inputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_3");
  QLineEdit *outputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_4");
  QLineEdit *varName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_5");
  QLineEdit *typeEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_6");
  QLineEdit *valueEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_7");
  QLineEdit *fsmName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_2");
  QLabel *fsmLabel = ui->groupBox_3->findChild<QLabel *>("labelFSM");

  inputsLine->clear();
  outputsLine->clear();
  varName->clear();
  typeEdit->clear();
  valueEdit->clear();
  fsmName->setText(fsm->getName());
  fsmLabel->setAlignment(Qt::AlignCenter);
  fsmLabel->setText(fsm->getName());
  

}

//aka clicking on state, calls this with pointer to that state
void MainWindow::updateStateInfo(StateItem *state) {
  selectedState = state;
  ui->groupBox->setEnabled(true);
  ui->groupBox->setVisible(true);
  ui->groupBox_3->setEnabled(false);
  ui->groupBox_3->setVisible(false);
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

  /* pottentially useless now redoing design
  QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  textEdit->clear();
  for (QString in : fsm->getInputs()) { textEdit->append(in); }

  QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  textEdit1->clear();
  for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

  QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  textEdit2->clear();
  for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName()); }
  */

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

void MainWindow::resizeCode() {
  QTextEdit *textEdit = ui->groupBox->findChild<QTextEdit *>("textEdit");
  //if (!textEdit) {return;}

  if (textEdit->pos() == QPoint(40, 90)) {
    textEdit->move(10, 90);
    textEdit->resize(331, 311);
    ui->pushButton->move(10, 410);
    return;
  }
  if (textEdit) {
    ui->pushButton->move(10, 90);
    textEdit->move(40, 90);
    textEdit->resize(645, 425);
    textEdit->raise();
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

void MainWindow::stateChanged(QString stateName) {
  for (QGraphicsItem *item : automatView->scene()->items()) {
    if (typeid(*item) != typeid(StateItem)) {continue;}
    StateItem *state = dynamic_cast<StateItem *>(item);
    if (state->state->getName() == stateName) {
      state->setBrush(Qt::green);
      state->setPen(QPen(Qt::black, 2));
    } else {
      state->setBrush(Qt::cyan);
      state->setPen(QPen(Qt::black, 1));
    }
  }
}

void MainWindow::printoutput(const QString &name, const QString &value) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": " + "[OUTPUT] " + name + " = " + value);
}
void MainWindow::timerstart(const QString &from, const QString &to, const QString &ms) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": Timer started from " + from + " to " + to + " for " + ms + " ms");
  for (QGraphicsItem *item : automatView->scene()->items()) {
    if (typeid(*item) != typeid(TransitionItem)) {continue;}
    TransitionItem *trans = dynamic_cast<TransitionItem *>(item);
    if (trans->getFrom()->state->getName() == from && trans->getTo()->state->getName() == to) {
      trans->startBlinking();
    }
  }
}
void MainWindow::timerend(const QString &from, const QString &to) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": Timer ended, transitioning from " + from + " to " + to);
  for (QGraphicsItem *item : automatView->scene()->items()) {
    if (typeid(*item) != typeid(TransitionItem)) {continue;}
    TransitionItem *trans = dynamic_cast<TransitionItem *>(item);
    if (trans->getFrom()->state->getName() == from && trans->getTo()->state->getName() == to) {
      trans->stopBlinking();
    }
  }
}
void MainWindow::printmsg(const QString &msg) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": [MESSAGE] " + msg);
}
void MainWindow::printerr(const QString &msg, const QString &code) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": [ATTENTION ERROR] " + msg + " (Error code: " + code + ")");
}
void MainWindow::printlog(const QString &msg) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": " + msg);
}

//mini parser for console commands
void MainWindow::onConsoleEnter() {
  QString command = ui->console->text();
  if (command.isEmpty()) return;
  QRegularExpression expressionCommand(R"(^\s*([a-zA-Z_][\w]*)\s*=\s*(.+)$)");
  QRegularExpressionMatch match = expressionCommand.match(command);
  if (match.hasMatch()) {
    QString identifier = match.captured(1);
    QString value = match.captured(2);
    client->sendSet(identifier, value);
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->logConsole->appendPlainText(now + ": " + identifier + " = " + value);
    ui->console->clear();
  } else {
    if (command == "run") {}
    else if (command == "help") {}
    else if (command == "version") {
    } else {
      QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
      ui->logConsole->appendPlainText(now + ": " + command);
      client->sendCall(command);
      ui->console->clear();
    }
  }
}

void MainWindow::saveVars() {
  QLineEdit *inputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_3");
  QLineEdit *outputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_4");
  QLineEdit *varName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_5");
  QLineEdit *typeEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_6");
  QLineEdit *valueEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_7");
  QLineEdit *fsmName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_2");
  QLabel *fsmLabel = ui->groupBox_3->findChild<QLabel *>("labelFSM");

  QString input = inputsLine->text();
  QString output = outputsLine->text();
  QString variable = varName->text();
  QString type = typeEdit->text();
  QString value = valueEdit->text();
  QString fsmNameText = fsmName->text();

  if (!(variable.isEmpty() || type.isEmpty() || value.isEmpty())) {
    fsm->addVariable(new Variable(type, variable, value));
  }

  input.isEmpty() ? input.clear() : fsm->addInput(input);
  output.isEmpty() ? output.clear() : fsm->addOutput(output);
  fsmNameText.isEmpty() ? fsmNameText.clear() : fsm->setName(fsmNameText);
  inputsLine->clear();
  outputsLine->clear();
  varName->clear();
  typeEdit->clear();
  valueEdit->clear();
  fsmLabel->setText(fsm->getName());

  QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  textEdit->clear();
  for (QString in : fsm->getInputs()) { textEdit->append(in); }

  QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  textEdit1->clear();
  for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

  QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  textEdit2->clear();
  for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName()); }
}

void MainWindow::editTransition(QListWidgetItem *item) {
  int row = ui->listWidget->row(item);
  if (row < 0 || row >= transitionItemsForSelectedState.size()) return;
  selectedTransition = transitionItemsForSelectedState[row];

  //ui->groupBox_2->setEnabled(true);
  QLineEdit *lineEdit = ui->groupBox->findChild<QLineEdit *>("eventLineEdit");
  QLineEdit *conditionEdit = ui->groupBox->findChild<QLineEdit *>("conditionLineEdit");
  QLineEdit *delayEdit = ui->groupBox->findChild<QLineEdit *>("delayLineEdit");
  QLineEdit *delayVarEdit = ui->groupBox->findChild<QLineEdit *>("delayVarLineEdit");
  lineEdit->setText(selectedTransition->transition->getEvent());
  conditionEdit->setText(selectedTransition->transition->getCondition());
  delayEdit->setText(QString::number(selectedTransition->transition->getDelay()));
  delayVarEdit->setText(selectedTransition->transition->getDelayVariableName());
}

void MainWindow::saveTransition() {
    if (!selectedTransition) return;

    QLineEdit *lineEdit = ui->groupBox->findChild<QLineEdit *>("eventLineEdit");
    QLineEdit *conditionEdit = ui->groupBox->findChild<QLineEdit *>("conditionLineEdit");
    QLineEdit *delayEdit = ui->groupBox->findChild<QLineEdit *>("delayLineEdit");
    QLineEdit *delayVarEdit = ui->groupBox->findChild<QLineEdit *>("delayVarLineEdit");

    selectedTransition->transition->setEvent(lineEdit->text());
    selectedTransition->transition->setCondition(conditionEdit->text());
    selectedTransition->setLabel(conditionEdit->text());
    selectedTransition->transition->setDelay(delayEdit->text().toInt());
    selectedTransition->transition->setDelayVariableName(delayVarEdit->text());
}

void MainWindow::loadFSM() {
  sudoclearFSM();
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
      QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
      textEdit->clear();
      for (QString in : fsm->getInputs()) { textEdit->append(in); }

      QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
      textEdit1->clear();
      for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

      QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
      textEdit2->clear();
      for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName()); }
  }
}

void MainWindow::runFSM() {
  QString xmlPath = QDir::temp().filePath("fsm_run.xml");
  XMLParser::FSMtoXML(*fsm, xmlPath);

  //code generation part, using codegen class
  CodeGenerator codeGen;
  QString generatedCode = codeGen.generateCode(fsm);
  QString genCpp = QDir::temp().filePath("fsm_generated.cpp");
  QFile file(genCpp);
  //writing to file
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << generatedCode;
    file.close();
  } else {
    qDebug() << "Failed to save generated code to file" << genCpp;
    return;
  }

  //compiling the generated code using g++
  //using process because compiler is not our class
  QString exe = QDir::temp().filePath("fsm_run");
  QProcess compiling;
  //missing qt5 libraries need to by added for compilation
  QStringList args = {genCpp, "-o", exe, "-fPIC", "-std=c++17"};
  QString pkgConfig = "`pkg-config --cflags --libs Qt5Core Qt5Network Qt5Widgets Qt5Xml`";
  QString command = QString("g++ %1 %2").arg(args.join(' '), pkgConfig);
  compiling.start("bash", QStringList() << "-c" << command);
  if (!compiling.waitForFinished() || compiling.exitCode() != 0) {
    qDebug() << "Compilation failed SADGE:" << compiling.readAllStandardError();
    return;
  }

  QProcess *serverProcess = new QProcess(this);
  serverProcess->start(exe, QStringList{});
  if (!serverProcess->waitForStarted()) {
    qDebug() << "Failed to start server process SADGE";
    return;
  }
  qDebug() << "Server is running RAAAAHHHH:" << serverProcess->processId();

  QTimer::singleShot(750, this, [this]() {
    client->connectToServer();
  });
}

void MainWindow::clearFSM() {
  auto answer = QMessageBox::question(this, "Clear FSM", "Are you sure you want to clear the FSM?", QMessageBox::Yes | QMessageBox::No);
  if (answer == QMessageBox::Yes) {
    sudoclearFSM();
  }
}

void MainWindow::sudoclearFSM() {
  automatView->scene()->clear();
  selectedState = nullptr;
  selectedTransition = nullptr;
  transitionItemsForSelectedState.clear();
  qDebug() << "Clearing FSM vars";
  //segfaulting here because some cast wont provide clear function
  auto clearWidget = [](QWidget* w) {
    if (auto line = qobject_cast<QLineEdit*>(w)) line->clear();
    else if (auto text = qobject_cast<QTextEdit*>(w)) text->clear();
    else if (auto list = qobject_cast<QListWidget*>(w)) list->clear();
  };
  for (QWidget *w : ui->groupBox->findChildren<QWidget*>()) {
    clearWidget(w);
  }
  qDebug() << "Clearing FSM groupbox";
  for (QWidget *w : ui->groupBox_3->findChildren<QWidget*>()) {
    clearWidget(w);
  }
  qDebug() << "Clearing FSM groupbox 3";
  ui->logConsole->clear();
  ui->listWidget->clear();
  ui->lineEdit->clear();
  ui->textEdit->clear();
  qDebug() << "Clearing FSM done";
  delete fsm;
  fsm = new FSM("Default FSM");
  qDebug() << "Clearing FSM done";
  showFSMInfo();
}

void MainWindow::refreshFSM() {
  QString tempPath = QDir::temp().filePath("fsm_refresh.xml");
  XMLParser::FSMtoXML(*fsm, tempPath);

  clearFSM();
  XMLParser::XMLtoFSM(tempPath, *fsm);

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
    QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
    textEdit->clear();
    for (QString in : fsm->getInputs()) { textEdit->append(in); }

    QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
    textEdit1->clear();
    for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

    QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
    textEdit2->clear();
    for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName()); }
}