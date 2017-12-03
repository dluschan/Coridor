#ifndef MYSERVER_H
#define MYSERVER_H

#include "mythread.h"
#include <QDebug>
#include <QTcpServer>
//#include <list>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject* parent = 0);
    void StartServer();
    std::list<MyThread*> players;
    std::list<Lobby*> lobbies;

private:
    void sendString(QString message, MyThread* thread);

signals:

public slots:
    void playerList(MyThread* thread);
    void createLobby(MyThread* thread, QString lobbyName);
    void lobbyList(MyThread* thread);

protected:
    void incomingConnection(int socketDescriptor);
};

#endif // MYSERVER_H
