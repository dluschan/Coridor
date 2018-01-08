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
	std::list<MyThread*> threads;
	std::list<Lobby*> lobbies;

private:
	void sendString(QString _message, MyThread* _thread);

signals:

public slots:
	// void playerList(MyThread* thread);
	void deletePlayer(MyThread* _thread);
	void createLobby(Lobby* _lobby);
	// void changeGameType(MyThread* _thread, int _gameType);
	void changeGameType(MyThread* _thread, int _gameType, int _status);
	void sendGameTypes(Player* _player, int _gameType, int _status);
	void deleteLobby(Lobby* _lobby);
	void deleteGuestLobby(Player* _player);
	void lobbiesList(MyThread* _thread);
	// void connectToLobby(Lobby* _lobby, Player* _player);
	void sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag);
	void sendConnectToLobbyHost(MyThread* i, Lobby* _lobby, Player* _player, bool _connectFlag);
	void sendRdy(Player* _host);
	// void sendStart(Player* _connectedPlayer);
	// void lobbyList(MyThread* thread);

protected:
	void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
