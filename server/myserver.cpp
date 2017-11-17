#include "myserver.h"

MyServer::MyServer() {}

MyServer::~MyServer() {}

void MyServer::startServer() {
  if (this->listen(QHostAddress::Any, 5555)) {
    qDebug() << "Listening";
  } else {
    qDebug() << "Not listening";
  }
}

void MyServer::incomingConnection(int socketDescriptor) {
  pSocket.push_back(new QTcpSocket(this));
  pSocket.back()->setSocketDescriptor(socketDescriptor);

  connect(pSocket.back(), SIGNAL(readyRead()), this, SLOT(sockReady()));
  connect(pSocket.back(), SIGNAL(disconnected()), this, SLOT(sockDisc()));

  qDebug() << socketDescriptor << "Client connected";

  pSocket.back()->write("You are connect");
  qDebug() << "Send client connect status - YES";
}

void MyServer::sockReady() {}

void MyServer::sockDisc() {
  qDebug() << "Disconnect";
  pSocket.back()->deleteLater();
}
