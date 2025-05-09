/**
 * @file GuiClient.hpp
 * @brief Header file for the GuiClient class for TCP communication with the FSM server.
 * @author xcsirim00
 * @date 8. May 2025
 */

#pragma once

#include <QDomDocument>
#include <QObject>
#include <QTcpSocket>

class GuiClient : public QObject {
    Q_OBJECT
   public:
    explicit GuiClient(const QString& host = "127.0.0.1", quint16 port = 54323, QObject* parent = nullptr);
    void connectToServer();
    void sendCommand(const QString& xml);
    void sendSet(const QString& name, const QString& value);
    void sendCall(const QString& name);
    void sendStatus();
    void sendHelp();
    void sendReqFSM();
    void sendDisconnect();
    void sendShutdown();
    void sendPong();

   public slots:
    void onReadyRead();

   private:
    QTcpSocket* socket;
    QString m_host;
    quint16 m_port;
};
