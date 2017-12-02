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

void MyServer::sendString(QString message, QTcpSocket* socket)
{
    message += "\n";
    QByteArray buffer(message.toStdString().c_str());
    socket->write(buffer);
    socket->waitForBytesWritten();
}

void MyServer::playerList(QTcpSocket* socket)
{
    QString message;
    for (const auto& i : players)
        message = i->player->name + " - status" + '\n';
    sendString(message, socket);
}

void MyServer::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << "Connecting...";
    players.push_back(new MyThread(socketDescriptor, this));
    connect(players.back(), SIGNAL(finished()), players.back(), SLOT(deleteLater()));
    connect(players.back(), SIGNAL(sendPlayerList(QTcpSocket*)), this, SLOT(playerList(QTcpSocket*)));
    players.back()->start();

    /* MyThread* thread = new MyThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start(); */
}
