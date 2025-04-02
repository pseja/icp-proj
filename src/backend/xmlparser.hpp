#pragma once

#include "logger.hpp"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QState>
#include <QStateMachine>

class XMLParser
{
  public:
    static bool parseXML(const QString &file_path, QStateMachine &state_machine);
};
