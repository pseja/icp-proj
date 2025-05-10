#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include "backend/fsm.hpp"
#include "backend/logger.hpp"
#include "backend/xmlparser.hpp"
#include "frontend/mainwindow.hpp" // for MainWindow
#include "backend/CodeGenerator.hpp"
#include "backend/GuiClient.hpp"
#include "backend/fsm.hpp"
#include "backend/xmlparser.hpp"
#include "frontend/mainwindow.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  qRegisterMetaType<FsmStatus>("FsmStatus");
  MainWindow w;
  w.show();

  qInstallMessageHandler(Logger::messageHandler);

  // // Create an FSM object
  // FSM fsm;

  // // Parse the XML file directly into the FSM object
  // if (!XMLParser::XMLtoFSM("examples/TOF5.xml", fsm)) {
  //   qDebug() << "Failed to parse XML file";
  //   return 1;
  // }

  // // Generate code from the FSM
  // CodeGenerator codeGen;
  // QString generatedCode = codeGen.generateCode(&fsm);

  // // Create the generated directory if it doesn't exist
  // QDir dir;
  // if (!dir.exists("generated")) {
  //   dir.mkdir("generated");
  // }

  // // Save the generated code to a file
  // QString filePath = QString("generated/%1_generated.cpp").arg(fsm.getName());
  // QFile file(filePath);
  // if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
  //   QTextStream out(&file);
  //   out << generatedCode;
  //   file.close();
  //   qDebug() << "Generated code saved to" << filePath;
  // } else {
  //   qDebug() << "Failed to save generated code to file" << filePath;
  // }
  // return 0;

  /*
  GuiClient* client = new GuiClient(&app);

  QTimer::singleShot(0, [client]() {
    client->connectToServer();
  });
  QTimer::singleShot(500, [client]() {
    client->sendHelp();
  });
  QTimer::singleShot(1000, [client]() {
    client->sendReqFSM();
  });
  QTimer::singleShot(1500, [client]() {
    client->sendSet("set_to", "3000");
  });
  QTimer::singleShot(2000, [client]() {
    client->sendSet("in", "1");
  });
  QTimer::singleShot(2500, [client]() {
    client->sendStatus();
  });
  QTimer::singleShot(3000, [client]() {
    client->sendSet("in", "0");
  });
  QTimer::singleShot(3500, [client]() {
    client->sendCall("req_rt");
  });
  QTimer::singleShot(4000, [client]() {
    client->sendStatus();
  });
  QTimer::singleShot(4500, [client]() {
    client->sendDisconnect();
  });
  QTimer::singleShot(5000, [client]() {
    client->connectToServer();
  });
  QTimer::singleShot(5500, [client]() {
    client->sendShutdown();
  });
  QTimer::singleShot(6000, &app, &QCoreApplication::quit);
  */
  return app.exec();
}
