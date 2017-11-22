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

    pSocket->write("Please, enter your name.");

    qDebug() << socketDescriptor << "Client Connected";

    exec();
}

void MyThread::readyRead()
{
    QString data;

    QDataStream in(pSocket);
    in.setVersion(QDataStream::Qt_4_0);
    in >> data;

    player = new Player(data, socketDescriptor);

    qDebug() << socketDescriptor << "Hello, " << data << "!";

    QByteArray login(data.toStdString().c_str());
    qDebug() << login;
    pSocket->write(login);
    pSocket->waitForBytesWritten();
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << "Client Disconnected";
    pSocket->deleteLater();
    exit(0);
}
