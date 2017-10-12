#include "myserver.h"

MyServer::MyServer(){}

MyServer::~MyServer(){}

void MyServer::startServer()
{
    if (this->listen(QHostAddress::Any,5555))
    {
        qDebug() << "Listening";
    }
    else
    {
        qDebug() << "Not listening";
    }
}

void MyServer::incomingConnection(int socketDescriptor)
{
    pSocket = new QTcpSocket(this);
    pSocket->setSocketDescriptor(socketDescriptor);

    connect(pSocket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(pSocket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

    qDebug()<<socketDescriptor<<"Client connected";

    pSocket->write("You are connect");
    qDebug() << "Send client connect status - YES";
}

void MyServer::sockReady()
{

}

void MyServer::sockDisc()
{
    qDebug() << "Disconnect";
    pSocket->deleteLater();
}
