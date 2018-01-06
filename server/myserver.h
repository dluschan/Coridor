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
	// void playerList(MyThread* thread);
	void deletePlayer(MyThread* thread);
	void createLobby(Lobby* lobby);
	void changeGameType(MyThread* thread, int _gameType);
	void deleteLobby(Lobby* lobby);
	void lobbiesList(MyThread* thread);
	// void lobbyList(MyThread* thread);

protected:
	void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
