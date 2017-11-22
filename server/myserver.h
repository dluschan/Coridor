#ifndef MYSERVER_H
#define MYSERVER_H

#include "mythread.h"
#include <QDebug>
#include <QTcpServer>
#include <list>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject* parent = 0);
    void StartServer();

private:
    std::list<MyThread*> players;

signals:

public slots:

protected:
    void incomingConnection(int socketDescriptor);
};

#endif // MYSERVER_H
