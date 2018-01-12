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

	Lobby* findLobby(Lobby* _lobby);

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
	void sendMessageSlot(QString _message, bool _errorFlag, QString _playerName);
	void sendFirstPlayerSlot(QString _firstPlayer, QString _guest, GameType _gameType);
	void sendQuitSlot(QString _reciever);
	void coridorSendQPointSlot(QPoint _point, bool _move, QString _enemy, bool _horizontal);
	void quartoSendQPointSlot(QPoint _point, int _figureId, QString _enemy);
	void quartoSendCheckWinSlot(QString _enemy, bool _checkWin);
	// void sendStart(Player* _connectedPlayer);
	// void lobbyList(MyThread* thread);

protected:
	void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
