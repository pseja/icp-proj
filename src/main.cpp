// #include "frontend/mainwindow.hpp"
#include "backend/xmlparser.hpp"
#include <QApplication>
#include <QStateMachine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // i don't need the window, i won't be showing anything anyway (i guess)
    // MainWindow w;
    // w.show();

    QStateMachine state_machine;
    QString xml_file_path = "examples/test.xml";

    if (!XMLParser::parseXML(xml_file_path, state_machine))
    {
        Logger::logError("Failed to parse XML in %s", xml_file_path.toUtf8().constData());
        return 1;
    }

    // state_machine.start();

    return app.exec();
}
