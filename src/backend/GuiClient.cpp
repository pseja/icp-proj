#include "GuiClient.hpp"

#include "logger.hpp"

GuiClient::GuiClient(const QString& host, quint16 port, QObject* parent)
    : QObject(parent), m_host(host), m_port(port) {
  socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::readyRead, this, &GuiClient::onReadyRead);
}

void GuiClient::connectToServer() {
  socket->connectToHost(m_host, m_port);
  if (socket->waitForConnected(3000)) {
    Logger::messageHandler(QtInfoMsg, {}, QString("Connected to %1:%2").arg(m_host).arg(m_port));
  } else {
    Logger::messageHandler(QtCriticalMsg, {}, "Failed to connect after 3 seconds: " + socket->errorString());
  }
}

void GuiClient::sendCommand(const QString& xml) {
  QByteArray msg = (xml + "\n").toUtf8();
  socket->write(msg);
  socket->flush();
  Logger::messageHandler(QtDebugMsg, {}, QString("Sent command: %1").arg(xml));
}

void GuiClient::sendSet(const QString& name, const QString& value) {
  QString xml = QString("<command type=\"set\"><name>%1</name><value>%2</value></command>")
                    .arg(name.toHtmlEscaped(), value.toHtmlEscaped());
  sendCommand(xml);
}

void GuiClient::sendCall(const QString& name) {
  QString xml = QString("<command type=\"call\"><name>%1</name></command>")
                    .arg(name.toHtmlEscaped());
  sendCommand(xml);
}

void GuiClient::sendStatus() {
  QString xml = "<command type=\"status\"></command>";
  sendCommand(xml);
}

void GuiClient::sendHelp() {
  QString xml = "<command type=\"help\"></command>";
  sendCommand(xml);
}

void GuiClient::sendReqFSM() {
  QString xml = "<command type=\"reqFSM\"></command>";
  sendCommand(xml);
}

void GuiClient::sendDisconnect() {
  QString xml = "<command type=\"disconnect\"></command>";
  sendCommand(xml);
}

void GuiClient::sendShutdown() {
  QString xml = "<command type=\"shutdown\"></command>";
  sendCommand(xml);
}

void GuiClient::onReadyRead() {
  while (socket->canReadLine()) {
    QString line = QString::fromUtf8(socket->readLine()).trimmed();
    if (line.isEmpty()) continue;
    QDomDocument prettyDoc;
    QString prettyXml = line;
    if (prettyDoc.setContent(line)) {
      prettyXml = prettyDoc.toString(2);
    }
    Logger::messageHandler(QtDebugMsg, {}, QString("Recieved command:\n%1").arg(prettyXml));
    QDomDocument doc;
    if (!doc.setContent(line)) {
      qWarning() << "Received malformed XML:" << line;
      continue;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() == "event") {
      QString type = root.attribute("type");
      if (type == "stateChange") {
        QString state = root.firstChildElement("name").text();
        qDebug() << "[STATE]" << state;
      } else if (type == "output") {
        QString name = root.firstChildElement("name").text();
        QString value = root.firstChildElement("value").text();
        qDebug() << "[OUTPUT]" << name << "=" << value;
      } else if (type == "timerStart") {
        QString from = root.firstChildElement("from").text();
        QString to = root.firstChildElement("to").text();
        QString ms = root.firstChildElement("ms").text();
        qDebug() << "[TIMER START] from" << from << "to" << to << ms << "ms";
      } else if (type == "timerExpired") {
        QString from = root.firstChildElement("from").text();
        QString to = root.firstChildElement("to").text();
        qDebug() << "[TIMER EXPIRED] from" << from << "to" << to;
      } else if (type == "fsm") {
        QString model = root.firstChildElement("model").text();
        qDebug() << "[FSM XML RECEIVED]";
        qDebug().noquote() << model;
      } else if (type == "log" || type == "disconnect" || type == "shutdown") {
        QString msg = root.firstChildElement("message").text();
        qDebug() << "[SERVER]" << msg;
      } else if (type == "error") {
        QString code = root.firstChildElement("code").text();
        QString msg = root.firstChildElement("message").text();
        qWarning() << "[ERROR] code:" << code << ", message:" << msg;
      } else if (type == "status") {
        QDomElement statusElem = root.firstChildElement("status");
        QString state = statusElem.firstChildElement("state").text();
        qDebug() << "[STATUS] State:" << state;
        QDomElement inputs = statusElem.firstChildElement("inputs");
        for (QDomElement input = inputs.firstChildElement("input"); !input.isNull(); input = input.nextSiblingElement("input")) {
          qDebug() << "  [INPUT]" << input.attribute("name") << "=" << input.text();
        }
        QDomElement outputs = statusElem.firstChildElement("outputs");
        for (QDomElement output = outputs.firstChildElement("output"); !output.isNull(); output = output.nextSiblingElement("output")) {
          qDebug() << "  [OUTPUT]" << output.attribute("name") << "=" << output.text();
        }
        QDomElement vars = statusElem.firstChildElement("variables");
        for (QDomElement var = vars.firstChildElement("var"); !var.isNull(); var = var.nextSiblingElement("var")) {
          QString varName = var.attribute("name");
          QString varType = var.attribute("type");
          QString varValue = var.text();
          qDebug() << "  [VAR]" << varName << "(" << varType << ") =" << varValue;
        }
        QDomElement timers = statusElem.firstChildElement("timers");
        for (QDomElement timer = timers.firstChildElement("timer"); !timer.isNull(); timer = timer.nextSiblingElement("timer")) {
          QString from = timer.firstChildElement("from").text();
          QString to = timer.firstChildElement("to").text();
          QString ms = timer.firstChildElement("ms").text();
          qDebug() << "  [TIMER] from" << from << "to" << to << "remaining:" << ms << "ms";
        }
      } else {
        qDebug() << "[EVENT] type=" << type << line;
      }
    } else {
      qDebug() << "[UNKNOWN XML]" << line;
    }
  }
}