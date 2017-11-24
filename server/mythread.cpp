#include "mythread.h"

MyThread::MyThread(int ID, QObject* parent)
    : QThread(parent)
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

    sendString("Please, enter your name.");
    c = (command)0;

    qDebug() << socketDescriptor << "Client Connected";

    exec();
}

void MyThread::readyRead()
{
    QString data;
    QStringList myOptions;
    myOptions << "-help"
              << "-player list";

    QDataStream in(pSocket);
    in.setVersion(QDataStream::Qt_4_0);
    in >> data;

    qDebug() << data;

    if (c != (command)0)
    {
        switch (myOptions.indexOf(data))
        {
        case 0:
            c = (command)1;
            break;
        case 1:
            c = (command)2;
            break;
        default:
            c = (command)3;
            sendString("Wrong command '" + data + "'.");
            break;
        }
    }

    switch (c)
    {
    case AskLogin:
        login(data);
        break;
    case Help:
        help();
        break;
    case AskPlayers:
        playerList();
        break;
    case WrongCommand:
        break;
    }
}

void MyThread::login(QString login)
{
    player = new Player(login, socketDescriptor);
    sendString("Hello, " + login + "! Type -help to ask for available commands");
    c = (command)1;
}

void MyThread::help()
{
    sendString("-help: ask for available commands");
    sendString("-player list: ask for player list");
}

void MyThread::playerList()
{
    /*while (!players.empty())
        sendString(players.back()->name + '/n');*/
    sendString("only uuuuuuuuuuuuuuuuu");
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << "Client Disconnected";
    pSocket->deleteLater();
    exit(0);
}

void MyThread::sendString(QString message)
{
    message += "\n";
    QByteArray buffer(message.toStdString().c_str());
    pSocket->write(buffer);
    pSocket->waitForBytesWritten();
}
