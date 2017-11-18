#include "mythread.h"

MyThread::MyThread(/*QTcpSocket* _pSocket,*/ int ID, QObject* parent)
    : QThread(parent)
//, pSocket(_pSocket)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    qDebug() << socketDescriptor << "Starting thread";
    pSocket = new QTcpSocket();
    if (!pSocket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(pSocket->error());
        return;
    }

    connect(pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

    qDebug() << socketDescriptor << "Client Connected";

    exec();
}

void MyThread::readyRead()
{
    QByteArray Data = pSocket->readAll();

    qDebug() << socketDescriptor << "Data in:" << Data;

    pSocket->write(Data);
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << "Client Disconnected";
    pSocket->deleteLater();
    exit(0);
}
