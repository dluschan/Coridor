#include "myserver.h"

MyServer::MyServer(QObject* parent)
    : QTcpServer(parent)
{
}

void MyServer::StartServer()
{
    if (!this->listen(QHostAddress::Any, 5555))
    {
        qDebug() << "Couldn't start the server";
    }
    else
    {
        qDebug() << "Listening...";
    }
}

void MyServer::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << "Connecting...";
    players.push_back(new MyThread(socketDescriptor, this));
    connect(players.back(), SIGNAL(finished()), players.back(), SLOT(deleteLater()));
    players.back()->start();

    /* MyThread* thread = new MyThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start(); */
}
