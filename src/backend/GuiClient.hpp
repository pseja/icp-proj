/**
 * @file GuiClient.hpp
 * @brief Header file for the GuiClient class for TCP communication with the FSM server.
 *
 * @author xcsirim00
 * @date 08-05-2025
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

    /**
     * @brief Set the host address for the FSM server.
     * @param host Hostname or IP address.
     */
    void setHost(const QString &host) { m_host = host; }

    /**
     * @brief Set the TCP port for the FSM server.
     * @param port TCP port number.
     */
    void setPort(quint16 port) { m_port = port; }

    /**
     * @brief Get the current host address for the FSM server.
     * @return Hostname or IP address as a QString.
     */
    QString getHost() const { return m_host; }

    /**
     * @brief Get the current TCP port for the FSM server.
     * @return TCP port number.
     */
    quint16 getPort() const { return m_port; }

    /**
     * @brief Check if the client is currently connected to the FSM server.
     * @return True if connected, false otherwise.
     */
    bool isConnected() const { return socket && socket->state() == QAbstractSocket::ConnectedState; }

   public slots:
    /**
     * @brief Handle incoming data from the FSM server.
     *
     * Processes all received XML events and commands.
     */
    void onReadyRead();

   signals:
    /**
     * @brief Emitted when the FSM state changes.
     * @param state The new state name.
     */
    void stateChange(const QString &state);
    /**
     * @brief Emitted when an output value is received from the FSM server.
     * @param name Name of the output.
     * @param value Value of the output.
     */
    void printoutput(const QString &name, const QString &value);
    /**
     * @brief Emitted when an input value is received from the FSM server.
     * @param name Name of the input.
     * @param value Value of the input.
     */
    void printinput(const QString &name, const QString &value);
    /**
     * @brief Emitted when a variable value is received from the FSM server.
     * @param name Name of the variable.
     * @param value Value of the variable.
     */
    void printvariable(const QString &name, const QString &value);
    /**
     * @brief Emitted when a timer is started in the FSM.
     * @param from Source state.
     * @param to Target state.
     * @param ms Duration in milliseconds.
     */
    void timerstart(const QString &from, const QString &to, const QString &ms);
    /**
     * @brief Emitted when a timer ends in the FSM.
     * @param from Source state.
     * @param to Target state.
     */
    void timerend(const QString &from, const QString &to);
    /**
     * @brief Emitted when a message is received from the FSM server.
     * @param msg The message text.
     */
    void printmsg(const QString &msg);
    /**
     * @brief Emitted when an error message is received from the FSM server.
     * @param msg The error message text.
     * @param code The error code.
     */
    void printerr(const QString &msg, const QString &code);
    /**
     * @brief Emitted when a log message is received from the FSM server.
     * @param msg The log message text.
     */
    void printlog(const QString &msg);
    /**
     * @brief Emitted when the FSM model XML is requested or received.
     * @param model The FSM model XML as a string.
     */
    void requestedFSM(const QString &model);
    /**
     * @brief Emitted when the FSM status is received from the server.
     * @param status The current FSM status.
     */
    void fsmStatus(const FsmStatus &status);
    /**
     * @brief Emitted when a shutdown message is sent from the FSM server.
     * @param msg The shutdown message.
     */
    void sendshutdown(const QString &msg);

   private:
    /**
     * @brief TCP socket used for communication with the FSM server.
     */
    QTcpSocket *socket;
    /**
     * @brief Hostname or IP address of the FSM server.
     */
    QString m_host;
    /**
     * @brief TCP port number of the FSM server.
     */
    quint16 m_port;
};
