/**
 * @file mainwindow.cpp
 * @brief Implements the MainWindow class and its methods, which is the primary
 *  GUI window for the Final state machine editor.
 *
 * @author Vaclav Sovak (xsovakv00)
 * @date 11-05-2025
 */

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
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qprocess.h>
#include <qradiobutton.h>
#include <qregularexpression.h>
#include <qtextedit.h>
#include <qtextlayout.h>
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
#include <QTcpSocket>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , autoConnectAttempted(false)
{
  //main window setup
  ui->setupUi(this);
  resize(1920, 1080);
  setMinimumSize(800, 600);

  //parts of the UI, instantiating, setting up
  fsm = new FSM("DefaultFSM");
  client = new GuiClient("127.0.0.1", 54323, this);
  automatView = new AutomatView(fsm, this);
  automatView->setMinimumSize(400, 300);
  automatView->setGeometry(710, 30, 1205, 960);

  //preparing consoles, setting up styles
  ui->logConsole->setStyleSheet("background-color:rgb(203, 202, 202);");
  ui->console->setStyleSheet("background-color:rgb(138, 138, 138);");
  
  //moving main fsm view
  ui->groupBox_3->setEnabled(true);
  ui->groupBox_3->setVisible(true);
  ui->groupBox_3->move(0, 0);

  //setting up rest of controls
  ui->groupBox->setVisible(false);
  ui->buttonStop->setEnabled(false);
  ui->buttonRun->setStyleSheet("background-color: green; color: white;");
  showFSMInfo();

  //init of label for fsm, idc its not pretty but automatview always redraws this
  ui->labelFSM = new QLabel(this);
  ui->labelFSM->setText("DefaultFSM");
  ui->labelFSM->setGeometry(1030, 50, 621, 51);
  ui->labelFSM->setStyleSheet("background: transparent; font-size: 36px;");
  ui->labelFSM->setAlignment(Qt::AlignCenter);
  ui->labelFSM->raise();
  ui->labelFSM->show();

  // Try to connect to an FSM server on startup
  autoConnectAttempted = true;
  connectToFSM();

  //-------------------------AUTOMAT VIEW SIGNALS-----------------
  connect(automatView, &AutomatView::showFSMInfo, this, &MainWindow::showFSMInfo);
  connect(automatView, &AutomatView::stateSelected, this,&MainWindow::updateStateInfo);
  connect(automatView, &AutomatView::transitionSelected, this, &MainWindow::updateTransitionInfo);
  connect(automatView, &AutomatView::addState, this, &MainWindow::addState);
  connect(automatView, &AutomatView::addTransition, this, &MainWindow::addTransition);
  //-------------------------------------------------------------

  //-------------------------UI SIGNALS--------------------------
  connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFSM);
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::loadFSM);
  connect(ui->actionTry_me, &QAction::triggered, this, &MainWindow::tryMe);
  connect(ui->actionCode_guidelines, &QAction::triggered, this, &MainWindow::codeGuidelines);
  connect(ui->actionAuthors, &QAction::triggered, this, &MainWindow::authors);
  connect(ui->actionExport_as_XML, &QAction::triggered, this, &MainWindow::exportXML);
  connect(ui->actionExport_as_cpp, &QAction::triggered, this, &MainWindow::exportCPP);
  connect(ui->actionAdd_connection, &QAction::triggered, this, &MainWindow::addConnection);
  connect(ui->actionCheck_connection, &QAction::triggered, this, &MainWindow::checkConnection);
  connect(ui->buttonRun, &QPushButton::pressed, this, &MainWindow::runFSM);
  connect(ui->pushButton, &QPushButton::pressed, this, &MainWindow::resizeCode);
  connect(ui->pushButton_2, &QPushButton::pressed, this, &MainWindow::deleteVar);
  connect(ui->buttonClear, &QPushButton::pressed, this, &MainWindow::clearFSM);
  connect(ui->buttonRefresh, &QPushButton::pressed, this, &MainWindow::refreshFSM);
  connect(ui->buttonStop, &QPushButton::pressed, this, &MainWindow::stopFSM);
  connect(ui->console, &QLineEdit::returnPressed, this, &MainWindow::onConsoleEnter);
  connect(ui->buttonBox_3, &QDialogButtonBox::accepted, this, &MainWindow::saveVars);
  connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this,&MainWindow::saveState);
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::saveTransition);
  connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::connectToFSM);
  //--------------------------------------------------------------

  //------------------------CLIENT SIGNALS------------------------
  connect(client, &GuiClient::stateChange, this, &MainWindow::stateChanged);
  connect(client, &GuiClient::printoutput, this, &MainWindow::printoutput);
  connect(client, &GuiClient::printinput, this, &MainWindow::printinput);
  connect(client, &GuiClient::printvariable, this, &MainWindow::printvariable);
  connect(client, &GuiClient::timerstart, this, &MainWindow::timerstart);
  connect(client, &GuiClient::timerend, this, &MainWindow::timerend);
  connect(client, &GuiClient::printmsg, this, &MainWindow::printmsg);
  connect(client, &GuiClient::printerr, this, &MainWindow::printerr);
  connect(client, &GuiClient::printlog, this, &MainWindow::printlog);
  connect(client, &GuiClient::requestedFSM, this, &MainWindow::requestedFSM);
  connect(client, &GuiClient::fsmStatus, this, &MainWindow::fsmStatus);
  connect(client, &GuiClient::sendshutdown, this, &MainWindow::handleshutdown);
  //---------------------------------------------------------------
}

// destructor, cleans up temp files
//terminates server process if running
MainWindow::~MainWindow() {
  automatView->disconnect();
  client->disconnect();

  if (serverProcess) {
    if (serverProcess->state() != QProcess::NotRunning) {
      serverProcess->terminate();
      serverProcess->waitForFinished(1000); // wait max 1 sec
    }
    delete serverProcess;
    serverProcess = nullptr;
  }
  cleanupTempFiles();
  delete ui;
}

//function to save the state of the FSM
//checks if the state is selected, if not, shows a warning
void MainWindow::saveState() {
  if (selectedState) {

    if (ui->lineEdit->text().isEmpty()) {
      QMessageBox::warning(this, "Error", "State name cannot be empty.");
      return;
    }

    for (State *state : fsm->getStates()) {
      if (state != selectedState->state && state->getName() == ui->lineEdit->text()) {
        QMessageBox::warning(this, "Error", "State name already exists.");
        return;
      }
    }
    //updating
    selectedState->updateState(ui->lineEdit->text(), ui->textEdit->toPlainText(), ui->radioButton_3->isChecked());
    if (ui->radioButton_3->isChecked()) {
      fsm->setInitialState(selectedState->state);
    }
    updateStateInfo(selectedState);
  }
}

//loasing information about the FSM into main fsm view
void MainWindow::showFSMInfo() {
  ui->groupBox->setEnabled(false);
  ui->groupBox->setVisible(false);
  ui->groupBox_2->setEnabled(false);
  ui->groupBox_2->setVisible(false);
  ui->groupBox_3->setEnabled(true);
  ui->groupBox_3->setVisible(true);
  
  //getting all fsm vars
  QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  textEdit->clear();
  for (QString in : fsm->getInputs()) { textEdit->append(in); }

  QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  textEdit1->clear();
  for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

  QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  textEdit2->clear();
  for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName() + " " + var->getType() + " = " + var->getValue().toString()); }
  
  QLineEdit *inputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_3");
  QLineEdit *outputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_4");
  QLineEdit *varName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_5");
  QLineEdit *typeEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_6");
  QLineEdit *valueEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_7");
  QLineEdit *fsmName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_2");
  QTextEdit *fsmComment = ui->groupBox_3->findChild<QTextEdit *>("textEdit_2");
  // QLabel *fsmLabel = ui->groupBox_3->findChild<QLabel *>("labelFSM");
  

  inputsLine->clear();
  outputsLine->clear();
  varName->clear();
  typeEdit->clear();
  valueEdit->clear();
  fsmComment->setText(fsm->getComment());
  fsmName->setText(fsm->getName());
  ui->labelFSM->setAlignment(Qt::AlignCenter);
  ui->labelFSM->setText(fsm->getName());
  
}

// aka clicking on state, calls this with pointer to that state
//shows the state info in the state view
void MainWindow::updateStateInfo(StateItem *state) {
  selectedState = state;
  ui->groupBox->setEnabled(true);
  ui->groupBox->setVisible(true);
  ui->groupBox_2->setEnabled(false);
  ui->groupBox_2->setVisible(false);
  ui->groupBox_3->setEnabled(false);
  ui->groupBox_3->setVisible(false);
  QLineEdit *lineEdit = ui->groupBox->findChild<QLineEdit *>("lineEdit");
  QLabel *labelName = ui->groupBox->findChild<QLabel *>("label_15");
  QTextEdit *textedit = ui->groupBox->findChild<QTextEdit *>("textEdit");
  QRadioButton *radio = ui->groupBox->findChild<QRadioButton *>("radioButton_3");
  if (lineEdit) {
    lineEdit->setText(selectedState->state->getName());
    labelName->setAlignment(Qt::AlignCenter);
    labelName->setText(selectedState->state->getName());
  }
  if (textedit) {
    textedit->setText(selectedState->state->getCode());
  }

bool otherInitial = false;
for (State *item : fsm->getStates()) {
    if (item != state->state && item->isInitial()) {
        otherInitial = true;
        break;
    }
}

//locking radio to prevent multiple initial states
if (state->state->isInitial()) {
    radio->setEnabled(true);
    radio->setChecked(true);
} else if (!otherInitial) {
    radio->setEnabled(true);
    radio->setChecked(false);
} else {
    radio->setEnabled(false);
    radio->setChecked(false);
}

// clear the list of transitions
// and add transitions for the selected state
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
        //figuring out the direction of the transition
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

//function to update the transition info into the transition view
void MainWindow::updateTransitionInfo(TransitionItem *transition) {
  selectedTransition = transition;
  if (!transition) {return;}
  ui->groupBox->setEnabled(false);
  ui->groupBox->setVisible(false);
  ui->groupBox_3->setEnabled(false);
  ui->groupBox_3->setVisible(false);
  ui->groupBox_2->setEnabled(true);
  ui->groupBox_2->setVisible(true);
  ui->groupBox_2->move(0, 0);

  //getting pointers to the UI elements
  QLineEdit *eventLine = ui->groupBox_2->findChild<QLineEdit *>("eventLineEdit");
  QTextEdit *conditionLine = ui->groupBox_2->findChild<QTextEdit *>("conditionEdit");
  QLineEdit *delayVarLine = ui->groupBox_2->findChild<QLineEdit *>("delayVarLineEdit");
  QLabel *fromLabel = ui->groupBox_2->findChild<QLabel *>("labelFrom");
  QLabel *toLabel = ui->groupBox_2->findChild<QLabel *>("labelTo");
  QLabel *arrowLabel = ui->groupBox_2->findChild<QLabel *>("labelArrow");
  //filing the UI elements with the transition info
  eventLine->clear();
  conditionLine->clear();
  delayVarLine->clear();
  eventLine->setText(transition->transition->getEvent());
  conditionLine->setText(transition->transition->getCondition());
  delayVarLine->setText(transition->transition->getDelayVariableName());
  fromLabel->setAlignment(Qt::AlignLeft);
  toLabel->setAlignment(Qt::AlignLeft);
  arrowLabel->setAlignment(Qt::AlignCenter);
  fromLabel->setText(transition->getFrom()->state->getName());
  toLabel->setText(transition->getTo()->state->getName());
  arrowLabel->setText("→");
}

//is trigered by clicking on the button in the state view, for code editation
//shows the code editor bigger
void MainWindow::resizeCode() {
  QTextEdit *textEdit = ui->groupBox->findChild<QTextEdit *>("textEdit");
  QLabel *labelName = ui->groupBox->findChild<QLabel *>("label");
  QLabel *labelName2 = ui->groupBox->findChild<QLabel *>("label_2");
  //if (!textEdit) {return;}
  //repositioning the text edit and the button
  if (textEdit->pos() == QPoint(40, 10)) {
    labelName->setVisible(true);
    labelName2->setVisible(true);
    textEdit->move(10, 190);
    textEdit->resize(671, 281);
    ui->pushButton->move(10, 480);
    return;
  }
  if (textEdit) {
    labelName->setVisible(false);
    labelName2->setVisible(false);
    ui->pushButton->move(10, 90);
    textEdit->move(40, 10);
    textEdit->resize(655, 500);
    textEdit->raise();
  }
}

// function to delete the selected state
// states automatically delete their transitions
void MainWindow::handleStateDeleted(StateItem *state) {
  fsm->removeState(state->state);

  QList<QGraphicsItem*> items = automatView->scene()->items();
  for (QGraphicsItem* item : items) {
      TransitionItem* t = dynamic_cast<TransitionItem*>(item);
      if (t && (t->getFrom() == state || t->getTo() == state)) {
        automatView->scene()->removeItem(t);
        //fsm->removeTransition(t->transition);
        delete t;
      }
  }
  automatView->scene()->removeItem(state);
  delete state;
}

// function to delete the selected transition
// transitions dont care about their states
void MainWindow::handleTransitionDeleted(TransitionItem *transition) {
  fsm->removeTransition(transition->transition);
  automatView->scene()->removeItem(transition);
  delete transition;
}

// function to clear the FSM(provisory)
void MainWindow::loadAutomat(const QVector<StateItem *> &states) {
  automatView->clearMask();
  for (StateItem *state : states) {
    //automatView->addState(state);
  }
}

// this is invoked when user creates new state
// it sets the state as initial if there is no other initial state
// and sets the pen color to purple
// it also connects the state to the deletion signal
void MainWindow::addState(StateItem *StateItem) {
  bool hasInitial = false;
  for (State *state : fsm->getStates()) {
    if (state->isInitial()) {hasInitial = true; break;}
  }
  if (!hasInitial) {
    StateItem->state->setInitial(true);
    StateItem->setPen(QPen(QColor(160, 32, 240), 5));
    fsm->setInitialState(StateItem->state);
  }else {
    StateItem->state->setInitial(false);
  }
  fsm->addState(StateItem->state);
  connect(StateItem, &StateItem::stateDeleted, this,&MainWindow::handleStateDeleted);
}

// function to add transition to the FSM
// it connects the transition to the deletion signal
void MainWindow::addTransition(TransitionItem *transition) {
  connect(transition, &TransitionItem::transitionDeleted, this, &MainWindow::handleTransitionDeleted);
  fsm->addTransition(transition->transition);
}

// this function is triggered when the user wants to save the FSM
// file action like usual is conected to this
// user can choose the file name and location
void MainWindow::saveFSM() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save FSM"), "",
                                                  tr("XML Files (*.xml)"));
  for (State *state : fsm->getStates()) {
    qDebug() << "State in FSM:" << state->getName();
  }
  if (!fileName.isEmpty()) {
    XMLParser::FSMtoXML(*fsm, fileName); //pseja parser
  }
}

// message received from FSM server about state change
// it sets the state color to green
// simulates active state in running FSM
void MainWindow::stateChanged(QString stateName) {
  if (autoConnectAttempted) {
    qDebug() << "Connected to running FSM server on startup";
    ui->logConsole->appendPlainText("Automatically connected to a remote FSM server.");
    autoConnectAttempted = false;
  }
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

// output message from server, last value is saved
// map of outputs is used to display the values in the UI
void MainWindow::printoutput(const QString &name, const QString &value) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": " + "[OUTPUT] " + name + " = " + value);
  QTextEdit *output = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  outputs.insert(name, value);
  output->clear();
  for (auto it = outputs.begin(); it != outputs.end(); ++it) {
    output->append(it.key() + " = " + it.value());
  }
}
//input message from server, last value is saved
// map of inputs is used to display the values in the UI
void MainWindow::printinput(const QString &name, const QString &value) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": " + "[INPUT] " + name + " = " + value);
  QTextEdit *input = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  inputs.insert(name, value);
  input->clear();
  for (auto it = inputs.begin(); it != inputs.end(); ++it) {
    input->append(it.key() + " = " + it.value());
  }
}
//variable message from server, last value is saved
// fsm saves variables and is used to display the values in the UI
void MainWindow::printvariable(const QString &name, const QString &value) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": " + "[VARIABLE] " + name + " = " + value);
  QTextEdit *variables = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  for (Variable *var : fsm->getVariables()) {
    if (var->getName() == name) {
      var->setValue(value);
    }
  }
  variables->clear();
  for (Variable *var : fsm->getVariables()) {
    variables->append(var->getName() + " = " + var->getValue().toString());
  }
}
// message of timer start signaling server has begin countdown
// appropriate transition is blinking until timer end comes
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
// message of timer end signaling server has finished countdown
// appropriate transition is not blinking anymore
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
// simple message from server
// it is used to display messages in the UI, log console
void MainWindow::printmsg(const QString &msg) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": [MESSAGE] " + msg);
}
// error message from server
// it is used to display error messages in the UI, log console
void MainWindow::printerr(const QString &msg, const QString &code) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": [ATTENTION ERROR] " + msg + " (Error code: " + code + ")");
}
// function to log messages in the UI, log console
void MainWindow::printlog(const QString &msg) {
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  ui->logConsole->appendPlainText(now + ": " + msg);
}
// function that provides user a structure of the FSM
// information is parsed and used to display the FSM in the UI
void MainWindow::requestedFSM(const QString &model) {
  qDebug() << "got FSM";
  ui->logConsole->appendPlainText("[FSM XML RECEIVED]");
  sudoclearFSM();
  QString user = QString::fromLocal8Bit(qgetenv("USER"));
  if (user.isEmpty()) user = "unknown";
  QString tempPath = QDir::temp().filePath(QString("req_fsm_%1.xml").arg(user));

  // extract <automaton>...</automaton> from the XML
  QRegularExpression re(R"(<automaton[\s\S]*?</automaton>)");
  QRegularExpressionMatch match = re.match(model);
  if (!match.hasMatch()) {
    qDebug() << "[ERROR] Could not find <automaton> element in model XML!";
    ui->logConsole->appendPlainText("[ERROR] Could not find <automaton> element in model XML!");
    return;
  }
  QString automatonXml = match.captured(0);

  QFile file(tempPath);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << automatonXml;
    file.close();
    qDebug() << "[DEBUG] FSM XML written to temp file:" << tempPath;
  } else {
    qDebug() << "[ERROR] Could not write FSM XML to temp file:" << tempPath;
    return;
  }
  if(!XMLParser::XMLtoFSM(tempPath, *fsm)){return;}
  ui->logConsole->appendPlainText("[FSM XML PARSED]");
  automatView->scene()->clear();
  int cols = ceil(sqrt(fsm->getStates().size()));
  int spacing = 120;
  int row = 0, col = 0;
  //displaying all states that came in the model
  QMap<State*, StateItem*> state_map;
  for (State *state : fsm->getStates()) {
    StateItem *stateItem = new StateItem(state->getName(), state->getCode());
    stateItem->state = state;
    stateItem->setPos(100 + col * spacing, 100 + row * spacing);
    if (state->isInitial()) { stateItem->setPen(QPen(QColor(160, 32, 240), 5));} 
    else {stateItem->setPen(QPen(Qt::black, 2));}
    automatView->scene()->addItem(stateItem);
    state_map[state] = stateItem;
    

    connect(stateItem, &StateItem::stateDeleted, this, &MainWindow::handleStateDeleted);
    if (++col >= cols) { col = 0; ++row; }
  }
  //displaying all transitions that came in the model
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
  qDebug() << "Requesting status from FSM";
  client->sendStatus();
  showFSMInfo();
}
// status message always comes after connecting to other running FSM
// it is used to display the FSM status in the UI and simulate the current running FSM
void MainWindow::fsmStatus(const FsmStatus &status) {
  ui->groupBox_3->setEnabled(false);
  QTextEdit *output = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  QTextEdit *input = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  QTextEdit *variables = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  output->clear();
  input->clear();
  variables->clear();

  //active state is set to green
  stateChanged(status.state);
  ui->logConsole->appendPlainText("[LAST ACTIVE STATE] " + status.state);
  //displaying all inputs, outputs, variables and timers
  for (auto it = status.inputs.begin(); it != status.inputs.end(); ++it) {
    ui->logConsole->appendPlainText("[LAST INPUT] " + it.key() + " = " + it.value());
    input->append(it.key() + " = " + it.value());
    inputs.insert(it.key(), it.value());
  }
  for (auto it = status.outputs.begin(); it != status.outputs.end(); ++it) {
    ui->logConsole->appendPlainText("[LAST OUTPUT] " + it.key() + " = " + it.value());
    output->append(it.key() + " = " + it.value());
    outputs.insert(it.key(), it.value());
  }
  for (auto it = status.variables.begin(); it != status.variables.end(); ++it) {
    ui->logConsole->appendPlainText("[LAST VARIABLE] " + it->name + " " + it->type + " = " + it->value);
    variables->append(it->name + " = " + it->value);
    Variable* var = fsm->getVariable(it->name);
    if (var) {
      var->setValue(it->value);
    } else {
      fsm->addVariable(new Variable(it->type, it->name, it->value));
    }
  }
  for (auto it = status.timers.begin(); it != status.timers.end(); ++it) {
    ui->logConsole->appendPlainText("[LAST TIMER] " + it->from + " -> " + it->to + " " + it->ms + " ms");
    timerstart(it->from, it->to, it->ms);
  }
}

// message from server about shutdown
// we announce the shutdown in the log console
// kill the process if running and ours otherwise sending shutdown back
void MainWindow::handleshutdown(const QString &msg) {
  ui->logConsole->appendPlainText("[SHUTDOWN] FSM server is shutting down.");
  stopFSM();
}

// mini parser for console commands
// user uses this to send commands to the FSM server and to control the FSM
// console supports our own commands to control the environment
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
    if (command == "run") {
      runFSM();
      ui->console->clear();
    } else if (command == "clear") {
      ui->logConsole->clear();
      ui->console->clear();
    } else if (command == "refresh") {
      refreshFSM();
      ui->console->clear();
    } else if (command == "help") {
      QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
      ui->logConsole->appendPlainText(now + ": ");
      ui->logConsole->appendPlainText(help);
      ui->console->clear();
    } else if (command == "version") {
      ui->logConsole->appendPlainText("[VERSION] Version: 1.0 alpha");
      ui->console->clear();
    } else {
      QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
      ui->logConsole->appendPlainText(now + ": " + command);
      client->sendCall(command);
      ui->console->clear();
    }
  }
}
// function to save the variables of the FSM
// its triggered by saving button in the fsm view
void MainWindow::saveVars() {
  QLineEdit *inputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_3");
  QLineEdit *outputsLine = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_4");
  QLineEdit *varName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_5");
  QLineEdit *typeEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_6");
  QLineEdit *valueEdit = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_7");
  QLineEdit *fsmName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_2");
  QTextEdit *fsmDescription = ui->groupBox_3->findChild<QTextEdit *>("textEdit_2");
  //QLabel *fsmLabel = ui->groupBox_3->findChild<QLabel *>("labelFSM");

  QString input = inputsLine->text();
  QString output = outputsLine->text();
  QString variable = varName->text();
  QString type = typeEdit->text();
  QString value = valueEdit->text();
  QString fsmNameText = fsmName->text();

  //variable cannot be empty
  if (!(variable.isEmpty() || type.isEmpty() || value.isEmpty())) {
    Variable *var = new Variable(type, variable, value);
    fsm->addVariable(var);
    //QMessageBox::information(this, "Error", "Variable requires name, type and value.");
  }

  input.isEmpty() ? input.clear() : fsm->addInput(input);
  output.isEmpty() ? output.clear() : fsm->addOutput(output);
  fsmNameText.isEmpty() ? fsmNameText.clear() : fsm->setName(fsmNameText);
  fsm->setComment(fsmDescription->toPlainText());
  inputsLine->clear();
  outputsLine->clear();
  varName->clear();
  typeEdit->clear();
  valueEdit->clear();
  ui->labelFSM->setText(fsm->getName());

  //reaload the variables information
  QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  textEdit->clear();
  for (QString in : fsm->getInputs()) { textEdit->append(in); }

  QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  textEdit1->clear();
  for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

  QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  textEdit2->clear();
  for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName() + " " + var->getType() + " = " + var->getValue().toString()); }
}

// function to delete the selected variable
// its triggered by delete button in the fsm view
// it asks for confirmation before deleting
// this can cause trouble, because transitions are not updated and will require variables
void MainWindow::deleteVar() {
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(
      this, "Delete Variable",
      "ATTENTION: After deleting this variable it will no longer be available "
      "in FSM transitions. DEFINITION OR CORRECTION REQUIRED. Do you want to "
      "proceed?",
      QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::No) {
    return;
  }
  QLineEdit *varName = ui->groupBox_3->findChild<QLineEdit *>("lineEdit_8");
  QString variable = varName->text();
  varName->clear();
  qDebug() << "Deleting variable: " << variable;
  //choosing which variable to delete
  if (variable.isEmpty()) return;
  for (Variable *var : fsm->getVariables()) {
    if (var->getName() == variable) {
      fsm->removeVariable(var);
    }
  }
  for (QString in : fsm->getInputs()) {
    if (in == variable) {
      fsm->removeInput(variable);
    }
  }
  for (QString out : fsm->getOutputs()) {
    if (out == variable) {
      fsm->removeOutput(variable);
    }
  }
  //reloading the variables information
  QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
  textEdit->clear();
  for (QString in : fsm->getInputs()) { textEdit->append(in); }

  QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
  textEdit1->clear();
  for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

  QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
  textEdit2->clear();
  for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName() + " " + var->getType() + " = " + var->getValue().toString()); }
}

// function to save the selected transition
// it is triggered by save button in the transition view
void MainWindow::saveTransition() {
  if (!selectedTransition) return;

  QLineEdit *lineEdit = ui->groupBox_2->findChild<QLineEdit *>("eventLineEdit");
  QTextEdit *conditionEdit = ui->groupBox_2->findChild<QTextEdit *>("conditionEdit");
  QLineEdit *delayVarEdit = ui->groupBox_2->findChild<QLineEdit *>("delayVarLineEdit");
  
  //no variable name nor input can be used for event if not declared
  QString event = lineEdit->text();
  if (!event.isEmpty() && !fsm->getInputs().contains(event)) {
      QMessageBox::warning(this, "Error", "Event name must be an input.");
      return;
  }
  QString delay = delayVarEdit->text();
  if (!delay.isEmpty() && !fsm->getVariables().contains(delay)) {
      QMessageBox::warning(this, "Error", "Variable must be declared.");
      return;
  }

  //updating the transition info
  selectedTransition->transition->setEvent(lineEdit->text());
  selectedTransition->transition->setCondition(conditionEdit->toPlainText());
  selectedTransition->setLabel(conditionEdit->toPlainText());
  selectedTransition->transition->setDelayVariableName(delayVarEdit->text());

  //process of of validating the delay variable
  //if the variable is empty, set delay to -1
  QString delayVarName = delayVarEdit->text();
  int delayValue = -1;
  if (!delayVarName.isEmpty()) {
      Variable* delayVar = fsm->getVariable(delayVarName);
      if (delayVar) {
          delayValue = delayVar->getValue().toInt();
          qDebug() << "Delay variable value: " << delayValue;
          // TODO: only allow setting this if the variable is defined and is an int
      }
  }
  selectedTransition->transition->setDelay(delayValue);
  updateTransitionInfo(selectedTransition);
  selectedTransition = nullptr;
}

// this function is used when the user wants to load a FSM from file
// in file action menu as open
void MainWindow::loadFSM() {
  // if no objects no need to clear
  if (!automatView->scene()->items().isEmpty()) {
    if (!clearFSM()) {return;}
  }
  //choosing the file to load
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open FSM"), "",
                                                  tr("XML Files (*.xml)"));
  if (!fileName.isEmpty()) {
    automatView->scene()->clear();
    if(!XMLParser::XMLtoFSM(fileName, *fsm)){return;}

    for (Variable *var : fsm->getVariables()) {
      if (var) { 
        var->resetToInitialValue();
      }
    }

    int cols = ceil(sqrt(fsm->getStates().size()));
    int spacing = 120;
    int row = 0, col = 0;
    // displaying all states that were parsed from the file
    // putting them in a basic position, user can move them manually
    QMap<State*, StateItem*> state_map;
    for (State *state : fsm->getStates()) {
      StateItem *stateItem = new StateItem(state->getName(), state->getCode());
      stateItem->state = state;
      stateItem->setPos(100 + col * spacing, 100 + row * spacing);

      if (state->isInitial()) { stateItem->setPen(QPen(QColor(160, 32, 240), 5));} 
      else {stateItem->setPen(QPen(Qt::black, 2));}
      
      automatView->scene()->addItem(stateItem);
      state_map[state] = stateItem;

      connect(stateItem, &StateItem::stateDeleted, this, &MainWindow::handleStateDeleted);
      if (++col >= cols) { col = 0; ++row; }
    }
    // displaying all transitions that were parsed from the file
    // putting them on existing states according to the file
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
    // displaying all inputs, outputs, variables
      QTextEdit *textEdit = ui->groupBox_3->findChild<QTextEdit *>("inputsEdit");
      textEdit->clear();
      for (QString in : fsm->getInputs()) { textEdit->append(in); }

      QTextEdit *textEdit1 = ui->groupBox_3->findChild<QTextEdit *>("outputsEdit");
      textEdit1->clear();
      for (QString out : fsm->getOutputs()) { textEdit1->append(out); }

      QTextEdit *textEdit2 = ui->groupBox_3->findChild<QTextEdit *>("variablesEdit");
      textEdit2->clear();
      for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName() + " " + var->getType() + " = " + var->getValue().toString()); }
    showFSMInfo();
  }
}

// user runs the fsm by clicking the big green button
// it is used to parse memory representation of the FSM
// creates temporary files for code generation and compilation
// creates a process for running the FSM
// and launches the FSM which also starts the server
// providing place to connect to for other clients
// ALL TEMPORARY FILES ARE DELETED AFTER RUNNING 
void MainWindow::runFSM() {
  showFSMInfo();
  ui->groupBox->setEnabled(false);
  ui->groupBox_2->setEnabled(false);
  ui->groupBox_3->setEnabled(false);
  ui->buttonClear->setEnabled(false);
  ui->buttonRun->setEnabled(false);
  ui->buttonRefresh->setEnabled(false);
  ui->buttonStop->setEnabled(true);
  ui->buttonRun->setStyleSheet("");
  ui->buttonStop->setStyleSheet("background-color: red; color: white;");
  automatView->setEnabled(false);
  automatView->setStyleSheet("QGraphicsView {"
                             "  background: rgb(255,255,255);"
                             "  color: black;"
                             "}"
                             "QGraphicsView:disabled {"
                             "  background: rgb(255,255,255);"
                             "  color: black;"
                             "}");
  for (QString name : fsm->getInputs()) {
    inputs.insert(name, "");
  }
  for (QString name : fsm->getOutputs()) {
    outputs.insert(name, "");
  }
  // system environment needed
  // using personal user login to create temp files
  // this is needed to run async with other clients which would use the same files
  QProcessEnvironment myenv = QProcessEnvironment::systemEnvironment();
  myenv.insert("LD_LIBRARY_PATH", "/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/lib");
  QString user = QString::fromLocal8Bit(qgetenv("USER"));
  if (user.isEmpty()) user = "unknown";
  QString xmlPath = QDir::temp().filePath("fsm_run_%1.xml").arg(user);
  XMLParser::FSMtoXML(*fsm, xmlPath);

  QString fileContents;
  QFile readFile(xmlPath);
  if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&readFile);
    fileContents = in.readAll();
    readFile.close();
    fsm->setInitialFSMXML(fileContents);
  } else {
    qDebug() << "[ERROR] Could not read FSM XML from temp file:" << xmlPath;
  }

  //code generation part, using codegen class
  CodeGenerator codeGen;
  QString generatedCode = codeGen.generateCode(fsm);
  QString genCpp = QDir::temp().filePath("fsm_generated_%1.cpp").arg(user);
  QFile file(genCpp);
  //writing to file
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << generatedCode;
    file.close();
  } else {
    ui->logConsole->appendPlainText("[ERROR] Code generation failed!");
    qDebug() << "Failed to save generated code to file" << genCpp;
    return;
  }

  //compiling the generated code using g++
  //using process because compiler is not our class
  QString exe = QDir::temp().filePath(QString("fsm_run_%1").arg(user));

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("PKG_CONFIG_PATH", "/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/lib/pkgconfig");
  QProcess pkgConfigProc;
  pkgConfigProc.setProcessEnvironment(env);
  pkgConfigProc.start("pkg-config", QStringList() << "--cflags" << "--libs" << "Qt5Core" << "Qt5Network"
                                                    << "Qt5Widgets" << "Qt5Xml" << "Qt5Gui");
  if (!pkgConfigProc.waitForFinished() || pkgConfigProc.exitCode() != 0) {
    ui->logConsole->appendPlainText("[ERROR] pkg-config failed!");
    qDebug() << "pkg-config failed:" << pkgConfigProc.readAllStandardError();
    return;
  }
  QString pkgFlags = QString::fromLocal8Bit(pkgConfigProc.readAllStandardOutput()).trimmed();
  QStringList flagList = pkgFlags.split(' ');
  // filter out flags that are used for compiling
  QStringList filteredFlags;
  for (const QString &flag : flagList) {
    if (flag.startsWith("-I/tmp/Qt5.9.2")) continue;
    if (flag.startsWith("-L/tmp/Qt5.9.2")) continue;
    filteredFlags << flag;
    qDebug() << "Filtered flag:" << flag;
  }
  filteredFlags << "-I/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/include";
  filteredFlags << "-L/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/lib";
  filteredFlags << "-lQt5Core" << "-lQt5Network" << "-lQt5Widgets" << "-lQt5Xml" << "-lQt5Gui";

  QStringList args = {genCpp, "-o", exe, "-fPIC", "-std=c++17"};
  args.append(filteredFlags);

  QString debugCmd = "g++ ";
  for (const QString &arg : args) debugCmd += arg + " ";
  qDebug() << "Compile command:" << debugCmd.trimmed();

  QProcess compiling;
  compiling.start("g++", args);
  if (!compiling.waitForFinished() || compiling.exitCode() != 0) {
    ui->logConsole->appendPlainText("[ERROR] Compilation failed!");
    qDebug() << "Compilation failed SADGE:" << compiling.readAllStandardError();
    return;
  }

  // QProcess *serverProcess = new QProcess(this);
  // killing the process if it is running (should never happen)
  if(serverProcess) {
    serverProcess->kill();
    serverProcess->deleteLater();
    serverProcess = nullptr;
  }
  serverProcess = new QProcess(this);
  serverProcess->setProcessEnvironment(myenv);
  serverProcess->start(exe, QStringList{"--port", QString::number(client->getPort()), "--host", client->getHost()});
  if (!serverProcess->waitForStarted()) {
    ui->logConsole->appendPlainText("[ERROR] Failed to start server process!");
    qDebug() << "Failed to start server process SADGE";
    return;
  }
  qDebug() << "Server is running RAAAAHHHH:" << serverProcess->processId();
  ui->logConsole->appendPlainText("[INFO] FSM server started.");
  QTimer::singleShot(750, this, [this]() {
    client->connectToServer();
  });
  stateChanged(fsm->getInitialState()->getName());
}
// function to stop the FSM
// it differs between terminating the process and sending shutdown command
// both client and owner handling
void MainWindow::stopFSM() {
  if (serverProcess) {
    if (serverProcess->state() != QProcess::NotRunning) {
      serverProcess->terminate();
      serverProcess->waitForFinished(1000); // wait max 1 sec
    }
    delete serverProcess;
    serverProcess = nullptr;
    ui->logConsole->appendPlainText("[INFO] FSM server stopped.");
  } else {
    if (client && client->isConnected()) {
      qDebug() << "[DEBUG] Sending shutdown command to remote FSM server.";
      client->sendShutdown();
    }
  }
  for (Variable *var : fsm->getVariables()) {
          if (var) { 
        var->resetToInitialValue();
      }
  }
  showFSMInfo();
  
  //stop blinking all transitions when fsm is stopped
  for (QGraphicsItem *item : automatView->scene()->items()) {
    if (typeid(*item) == typeid(TransitionItem)) {
      TransitionItem *trans = dynamic_cast<TransitionItem *>(item);
      trans->stopBlinking();      
    }
  }
  ui->groupBox->setEnabled(true);
  ui->groupBox_2->setEnabled(true);
  ui->groupBox_3->setEnabled(true);
  ui->buttonClear->setEnabled(true);
  ui->buttonRun->setEnabled(true);
  ui->buttonRefresh->setEnabled(true);
  ui->buttonStop->setEnabled(false);
  ui->buttonRun->setStyleSheet("background-color: green; color: white;");
  ui->buttonStop->setStyleSheet("");
  automatView->setEnabled(true);
}
// packaging for sudo clear FSM
// this depends on user decision
bool MainWindow::clearFSM() {
  auto answer = QMessageBox::question(this, "Clear FSM", "Are you sure you want to clear the FSM?", QMessageBox::Yes | QMessageBox::No);
  if (answer == QMessageBox::Yes) {
    sudoclearFSM();
    return true;
  } else {
    return false;
  }
  return false;
}
// function to clear the FSM
// it resets all variables and clears the FSM
// automatView is cleared
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
  fsm = new FSM("DefaultFSM");
  qDebug() << "Clearing FSM done";
  showFSMInfo();
}
// function to refresh the FSM
// this function could be very useful due to limitations of the FSM automatview
// sometimes user can push states out of the view
// or can bug shadowing of arrow redrawing
// refereshing keeps in untouched state, user doesnt lose progress but can reset
// the graphical representation
void MainWindow::refreshFSM() {
  bool hasInitial = false;
  for (State *state : fsm->getStates()) {
    if (state->isInitial()) { hasInitial = true; break; }
  }
  if (!hasInitial) {
    QMessageBox::warning(this, "Warning", "FSM has no initial state. Please set an initial state before refreshing.");
    return;
  }
  //using temporary file to store the FSM
  QString user = QString::fromLocal8Bit(qgetenv("USER"));
  if (user.isEmpty()) user = "unknown";
  QString tempPath = QDir::temp().filePath("fsm_refresh_%1.xml").arg(user);
  XMLParser::FSMtoXML(*fsm, tempPath);
  //clearing the automatView for any mistakes
  sudoclearFSM();
  if(!XMLParser::XMLtoFSM(tempPath, *fsm)){return;}

  int cols = ceil(sqrt(fsm->getStates().size()));
  int spacing = 120;
  int row = 0, col = 0;
  QMap<State*, StateItem*> state_map;
  for (State *state : fsm->getStates()) {
    StateItem *stateItem = new StateItem(state->getName(), state->getCode());
    stateItem->state = state;
    stateItem->setPos(100 + col * spacing, 100 + row * spacing);
    if (state->isInitial()) { stateItem->setPen(QPen(QColor(160, 32, 240), 5));} 
    else {stateItem->setPen(QPen(Qt::black, 2));}
    automatView->scene()->addItem(stateItem);
    state_map[state] = stateItem;

    connect(stateItem, &StateItem::stateDeleted, this, &MainWindow::handleStateDeleted);
    if (++col >= cols) { col = 0; ++row; }
  }

  //getting all states and transitions and placing them in the automatView
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
    for (Variable *var : fsm->getVariables()) { textEdit2->append(var->getName() + " " + var->getType() + " = " + var->getValue().toString()); }
}

// function to check the connection to the FSM server
// can be used in debugging cases or just user awarness
void MainWindow::checkConnection() {
  if (client->isConnected()) {
    QMessageBox::information(this, "Connection Status", "Connected to FSM server.");
  } else {
    QMessageBox::warning(this, "Connection Status", "Not connected to FSM server.");
  }
}
// function prepares the connection to the FSM server
// if connection is established, it asks for the fsm structure
void MainWindow::connectToFSM() {
  client->connectToServer();
  if (!client->isConnected()) {
    ui->logConsole->appendPlainText("[ERROR] Could not connect to FSM server.");
    return;
  }
  showFSMInfo();
  ui->groupBox->setEnabled(false);
  ui->groupBox_2->setEnabled(false);
  ui->groupBox_3->setEnabled(false);
  ui->buttonClear->setEnabled(false);
  ui->buttonRun->setEnabled(false);
  ui->buttonRefresh->setEnabled(false);
  ui->buttonStop->setEnabled(true);
  ui->buttonRun->setStyleSheet("");
  ui->buttonStop->setStyleSheet("background-color: red; color: white;");
  automatView->setEnabled(false);
  automatView->setStyleSheet("QGraphicsView {"
                             "  background: rgb(255,255,255);"
                             "  color: black;"
                             "}"
                             "QGraphicsView:disabled {"
                             "  background: rgb(255,255,255);"
                             "  color: black;"
                             "}");
  // Prepare input/output maps
  inputs.clear();
  outputs.clear();
  for (QString name : fsm->getInputs()) {
    inputs.insert(name, "");
  }
  for (QString name : fsm->getOutputs()) {
    outputs.insert(name, "");
  }
  client->sendReqFSM();
}
// function to cleanup the temporary files
// this is used to delete the files after running the FSM
void MainWindow::cleanupTempFiles() {
  QString user = QString::fromLocal8Bit(qgetenv("USER"));
  if (user.isEmpty()) user = "unknown";
    QStringList files = {
    QDir::temp().filePath(QString("fsm_run_%1.xml").arg(user)),
    QDir::temp().filePath(QString("fsm_generated_%1.cpp").arg(user)),
    QDir::temp().filePath(QString("fsm_run_%1").arg(user)),
    QDir::temp().filePath(QString("fsm_refresh_%1.xml").arg(user)),
    QDir::temp().filePath(QString("req_fsm_%1.xml").arg(user))
  };
  for (const QString &file : files) {
    if (QFile::exists(file)) {
      QFile::remove(file);
    }
  }
}
// provides the user with a dialog to set the connection to the FSM server
// connection is defautly set to localhost:54323 
void MainWindow::addConnection() {
  QDialog dialog(this);
  dialog.setWindowTitle("Add Connection configuration");
  dialog.resize(300, 120);

  QVBoxLayout *layout = new QVBoxLayout(&dialog);

  QLineEdit *hostEdit = new QLineEdit(&dialog);
  hostEdit->setPlaceholderText("Host");
  hostEdit->setText(client->getHost());

  QLineEdit *portEdit = new QLineEdit(&dialog);
  portEdit->setPlaceholderText("Port");
  portEdit->setText(QString::number(client->getPort()));
  portEdit->setValidator(new QIntValidator(1, 65535, portEdit));

  layout->addWidget(new QLabel("Host:", &dialog));
  layout->addWidget(hostEdit);
  layout->addWidget(new QLabel("Port:", &dialog));
  layout->addWidget(portEdit);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
  layout->addWidget(buttonBox);

  connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

  if (dialog.exec() == QDialog::Accepted) {
    QString host = hostEdit->text();
    int port = portEdit->text().toInt();
    client->setHost(host);
    client->setPort(port);
    ui->logConsole->appendPlainText("[INFO] New Host set: " + host + ", port: " + QString::number(port));
  }
}
// this is a little easter egg we made with colleagues
// its just a funny picture to make your day better
void MainWindow::tryMe() {
  //qDebug() << "Current working dir:" << QDir::currentPath();
  QPixmap pixmap(QCoreApplication::applicationDirPath() + "/../src/icons/koteseni.png");
  QDialog *dialog = new QDialog(this);
  QVBoxLayout *layout = new QVBoxLayout(dialog);
  QLabel *label = new QLabel(dialog);
  label->setPixmap(pixmap);
  label->setScaledContents(true);
  label->setFixedSize(600, 400);
  layout->addWidget(label);
  dialog->setWindowTitle("Kotěšení");
  dialog->resize(600, 400);
  dialog->exec();
}
//provides the user with a dialog of the code guidelines
void MainWindow::codeGuidelines() {
  QDialog *dialog = new QDialog(this);
  dialog->setWindowTitle("Code Guidelines");
  dialog->resize(700, 600);

  QVBoxLayout *layout = new QVBoxLayout(dialog);

  QTextEdit *textEdit = new QTextEdit(dialog);
  textEdit->setReadOnly(true);
  textEdit->setPlainText(helpText);
  textEdit->setFontFamily("monospace");
  textEdit->setFontPointSize(11);

  layout->addWidget(textEdit);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
  connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
  layout->addWidget(buttonBox);

  dialog->exec();
}
// provides the user with a dialog of the authors
void MainWindow::authors() {
  QDialog *dialog = new QDialog(this);
  dialog->setWindowTitle("Authors");
  dialog->resize(500, 250);

  QVBoxLayout *layout = new QVBoxLayout(dialog);

  QTableWidget *table = new QTableWidget(dialog);
  table->setColumnCount(3);
  table->setHorizontalHeaderLabels(QStringList() << "Name" << "xlogin" << "Role");
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  table->verticalHeader()->setVisible(false);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setSelectionMode(QAbstractItemView::NoSelection);

  struct Author { QString name, xlogin, role; };
  QVector<Author> authors = {
      {"Matúš Csirik", "xcsirim00", "Project Leader, Code gen, Client"},
      {"Lukáš Pšeja", "xpsejal00", "FSM, Logger"},
      {"Václav Sovák", "xsovakv00", "GUI"}
  };

  table->setRowCount(authors.size());
  for (int i = 0; i < authors.size(); ++i) {
      table->setItem(i, 0, new QTableWidgetItem(authors[i].name));
      table->setItem(i, 1, new QTableWidgetItem(authors[i].xlogin));
      table->setItem(i, 2, new QTableWidgetItem(authors[i].role));
  }

  layout->addWidget(table);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
  connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
  layout->addWidget(buttonBox);

  dialog->exec();
}
// function to export the FSM to XML without any compilation or running
void MainWindow::exportXML() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export FSM as XML"), "",
                                                    tr("XML Files (*.xml)"));
  if (!fileName.isEmpty()) {
    XMLParser::FSMtoXML(*fsm, fileName);
    ui->logConsole->appendPlainText("[INFO] FSM exported as XML: " + fileName);
  }
}
// function to export the FSM to C++ code, without any compilation or running
void MainWindow::exportCPP() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export FSM as C++"), "",
                                                    tr("C++ Files (*.cpp)"));
  if (!fileName.isEmpty()) {
    CodeGenerator codeGen;
    QString generatedCode = codeGen.generateCode(fsm);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out(&file);
      out << generatedCode;
      file.close();
      ui->logConsole->appendPlainText("[INFO] FSM exported as C++: " + fileName);
    } else {
      ui->logConsole->appendPlainText("[ERROR] Failed to export FSM as C++!");
    }
  }
}