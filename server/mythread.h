#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "player.h"
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QtNetwork>
#include <iostream>
#include <string>

enum command
{
    AskLogin,
    Help,
    AskPlayers,
    WrongCommand
};

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int ID, QObject* parent = 0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket* pSocket;
    int socketDescriptor;
    Player* player;
    command c;

    void login(QString login);
    void help();
    void playerList(/*std::list<Player*> players*/);
    void sendString(QString message);
};

#endif // MYTHREAD_H
