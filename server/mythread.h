#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "player.h"
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QtNetwork>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(/*QTcpSocket* _pSocket,*/ int ID, QObject* parent = 0);
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
};

#endif // MYTHREAD_H
