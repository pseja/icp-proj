#include "frontend/mainwindow.hpp"
#include "backend/CodeGen.hpp"
#include "backend/xmlparser.hpp"
#include "backend/fsm.hpp"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // Create an FSM object
    FSM fsm;
    
    // Parse the XML file directly into the FSM object
    if (!XMLParser::XMLtoFSM("examples/TOF5.xml", fsm)) {
        qDebug() << "Failed to parse XML file";
        return 1;
    }
    
    // Generate code from the FSM
    CodeGen codeGen;
    QString generatedCode = codeGen.generateCode(&fsm);
    
    // Create the generated directory if it doesn't exist
    QDir dir;
    if (!dir.exists("generated")) {
        dir.mkdir("generated");
    }
    
    // Save the generated code to a file
    QString filePath = QString("generated/%1_generated.cpp").arg(fsm.getName());
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << generatedCode;
        file.close();
        qDebug() << "Generated code saved to" << filePath;
    } else {
        qDebug() << "Failed to save generated code to file" << filePath;
    }
    
    return 0;
}
