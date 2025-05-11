#include <QApplication>

#include "backend/GuiClient.hpp"
#include "backend/logger.hpp"
#include "frontend/mainwindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<FsmStatus>("FsmStatus");
    MainWindow w;
    w.show();

    qInstallMessageHandler(Logger::messageHandler);

    return app.exec();
}
