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
    // pSocket.push_back(new QTcpSocket());
    MyThread* thread = new MyThread(/*pSocket.back(),*/ socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    /*QString login(pSocket.back()->readAll());
    players.push_back(new Player(login, socketDescriptor));
    const char* log = players.back()->name.toStdString().c_str();
    pSocket.back()->write("Hello, ");
    pSocket.back()->write(log);*/
}
