#include "frontend/mainwindow.hpp"
#include "backend/CodeGen.hpp"
#include "backend/MockStateMachine.hpp"
#include "backend/fsm.hpp"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // Create the mock state machine using the FSM wrapper class
    FSM* fsm = createTOF5sMachine();
    
    // Generate the code using CodeGen
    CodeGen codeGen;
    QString generatedCode = codeGen.generateCode(fsm);
    
    // Save the generated code to a file
    QFile file("TOF5s_generated.cpp");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << generatedCode;
        file.close();
        qDebug() << "Generated code saved to TOF5s_generated.cpp";
    } else {
        qDebug() << "Failed to save generated code to file";
    }
    
    // Clean up
    delete fsm;
    
    return 0;
}
