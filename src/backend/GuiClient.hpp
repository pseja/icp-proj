/**
 * @file GuiClient.hpp
 * @brief Header file for the GuiClient class for TCP communication with the FSM server.
 *
 * @author xcsirim00
 * @date 8. May 2025
 */

#pragma once

#include <QDomDocument>
#include <QObject>
#include <QTcpSocket>

struct FsmStatus {
    QString state;
    QMap<QString, QString> inputs;
    QMap<QString, QString> outputs;
    struct Variable {
        QString name, type, value;
    };
    QList<Variable> variables;
    struct Timer {
        QString from, to, ms;
    };
    QList<Timer> timers;
};
Q_DECLARE_METATYPE(FsmStatus)

class GuiClient : public QObject {
    Q_OBJECT
   public:
    /**
     * @brief Construct a new GuiClient object for TCP communication.
     *
     * @param host Hostname or IP address of the FSM server.
     * @param port TCP port of the FSM server.
     * @param parent Optional parent QObject.
     */
    explicit GuiClient(const QString &host = "127.0.0.1", quint16 port = 54323, QObject *parent = nullptr);

    /**
     * @brief Connect to the FSM server.
     *
     * Attempts to establish a TCP connection to the configured host and port.
     * Logs the result of the connection attempt.
     */
    void connectToServer();

    /**
     * @brief Send a raw XML command to the FSM server.
     *
     * @param xml The XML command to send.
     */

    void sendCommand(const QString &xml);

    /**
     * @brief Send a 'set' command to the FSM server.
     *
     * @param name Name of the input.
     * @param value Value to set.
     */
    void sendSet(const QString &name, const QString &value);

    /**
     * @brief Send a 'call' command to the FSM server.
     *
     * @param name Name of the input to call.
     */
    void sendCall(const QString &name);

    /**
     * @brief Request the current status from the FSM server.
     */
    void sendStatus();

    /**
     * @brief Request help information from the FSM server.
     */
    void sendHelp();

    /**
     * @brief Request the FSM model XML from the server.
     */
    void sendReqFSM();

    /**
     * @brief Send a disconnect command to the FSM server.
     */
    void sendDisconnect();

    /**
     * @brief Send a shutdown command to the FSM server.
     */
    void sendShutdown();

    /**
     * @brief Send a pong response to the FSM server.
     */
    void sendPong();

    void setHost(const QString &host) { m_host = host; }

    void setPort(quint16 port) { m_port = port; }

    QString getHost() const { return m_host; }

    quint16 getPort() const { return m_port; }

    bool isConnected() const { return socket && socket->state() == QAbstractSocket::ConnectedState; }

   public slots:
    /**
     * @brief Handle incoming data from the FSM server.
     *
     * Processes all received XML events and commands.
     */
    void onReadyRead();

   signals:
    void stateChange(const QString &state);
    void printoutput(const QString &name, const QString &value);
    void printinput(const QString &name, const QString &value);
    void printvariable(const QString &name, const QString &value);
    void timerstart(const QString &from, const QString &to, const QString &ms);
    void timerend(const QString &from, const QString &to);
    void printmsg(const QString &msg);
    void printerr(const QString &msg, const QString &code);
    void printlog(const QString &msg);
    void requestedFSM(const QString &model);
    void fsmStatus(const FsmStatus &status);
    void sendshutdown(const QString &msg);

   private:
    QTcpSocket *socket;
    QString m_host;
    quint16 m_port;
};
