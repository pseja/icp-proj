#include "xmlparser.hpp"

bool XMLParser::parseXML(const QString &file_path, QStateMachine &state_machine)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Logger::logError("Couldn't open file %s", file_path.toUtf8().constData());
        return false;
    }

    QDomDocument document;
    if (!document.setContent(&file))
    {
        Logger::logError("Couldn't set content of file %s", file_path.toUtf8().constData());
        file.close();
        return false;
    }

    file.close();

    QDomElement root = document.documentElement();
    if (root.tagName() != "automaton")
    {
        Logger::logError("Invalid XML format: Root element is not <automaton>");
        return false;
    }

    return true;
}