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
	std::list<MyThread*> players;
	std::list<Lobby*> lobbies;

private:
	void sendString(QString message, QTcpSocket* socket);

signals:

public slots:
	void playerList(QTcpSocket* socket);

protected:
	void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
