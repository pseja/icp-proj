#ifndef GUICLIENT_HPP
#define GUICLIENT_HPP

#include <QDomDocument>
#include <QObject>
#include <QTcpSocket>

class GuiClient : public QObject {
  Q_OBJECT
 public:
  explicit GuiClient(QObject* parent = nullptr);
  void connectToServer();
  void sendCommand(const QString& xml);
  void sendSet(const QString& name, const QString& value);
  void sendCall(const QString& name);
  void sendStatus();
  void sendHelp();
  void sendReqFSM();
  void sendDisconnect();
  void sendShutdown();

 public slots:
  void onReadyRead();

 private:
  QTcpSocket* socket;
};

#endif  // GUICLIENT_HPP
