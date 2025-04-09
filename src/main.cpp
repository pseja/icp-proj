#include "backend/fsm.hpp"
#include "backend/logger.hpp"
#include "backend/xmlparser.hpp"
#include "frontend/mainwindow.hpp" // for MainWindow
#include <QApplication>            // for QApplication

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    qInstallMessageHandler(Logger::messageHandler);

    // FSM state_machine = new FSM("test");

    // QString xml_file_path = "examples/TOF5.xml";

    // if (!XMLParser::XMLtoFSM(xml_file_path, state_machine))
    // {
    //     qCritical() << "Failed to parse XML in" << xml_file_path;
    //     return 1;
    // }

    // QString export_file_path = "examples/TOF5s_exported.xml";
    // if (!XMLParser::FSMtoXML(state_machine, export_file_path))
    // {
    //     qCritical() << "Failed to export FSM to" << export_file_path;
    //     return 1;
    // }

    return app.exec();
}
