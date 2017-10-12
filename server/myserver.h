#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class MyServer: public QTcpServer
{
    Q_OBJECT

public:
    MyServer();
    ~MyServer();

    QTcpSocket* pSocket;
    QByteArray Data;

public slots:
    void startServer();
    void incomingConnection(int socketDescriptor);
    void sockReady();
    void sockDisc();

};

#endif // MYSERVER_H
