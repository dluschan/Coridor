#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include <list>

class MyServer : public QTcpServer {
  Q_OBJECT

public:
  MyServer();
  ~MyServer();

  std::list<QTcpSocket *> pSocket;
  QByteArray Data;

public slots:
  void startServer();
  void incomingConnection(int socketDescriptor);
  void sockReady();
  void sockDisc();
};

#endif // MYSERVER_H
