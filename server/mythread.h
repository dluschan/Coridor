#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "../common/CoridorLogic.h"
#include "../common/command.h"
#include "../common/quartologic.h"
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QtNetwork>
#include <exception>
#include <iostream>
#include <string>

class MyThread : public QThread
{
	Q_OBJECT
public:
	explicit MyThread(int ID, QObject* parent = 0);
	QTcpSocket* pSocket;
	// int socketDescriptor;
	Player* pPlayer;
	Lobby* pLobby = new Lobby();
	void run();
	void sendMessage(QString message, bool error);
	Lobby* createLobby(QString lobbyName, int gameType);
	void* deleteGuestLobby(Lobby* lobby);
	void write(QByteArray buffer);
	void sendRdy();
	void sendStart();
	void sendFirstPlayer(QString _firstPlayer, QString _guest);
	// Player* createPlayer(QString playerName);

signals:
	void deletePlayerSignal(MyThread* thread);
	void errorSignal(QTcpSocket::SocketError socketError);
	void sendPlayerListSignal(MyThread* thread);
	void createLobbySignal(Lobby* pLobby);
	// void changeGameTypeSignal(MyThread* thread, int gameType);
	void changeGameTypeSignal(MyThread* thread, int gameType, int _status);
	void sendGameTypesSignal(Player* player, int gameType, int _status);
	void deleteLobbySignal(Lobby* pLobby);
	void deleteGuestLobbySignal(Player* pPlayer);
	void connectToLobbySignal(Lobby* pLobby, Player* player, bool connectFlag);
	void connectToHostLobbySignal(MyThread* thread, Lobby* pLobby, Player* player, bool connectFlag);
	void sendLobbiesListSignal(MyThread* thread);
	void sendRdySignal(Player* host);
	void sendFirstPlayerSignal(QString firstPlayer, QString guest);
	// void sendStartSignal(Player* connectedPlayer);

public slots:
	void readyRead();
	void disconnected();
	void changeGameType(int _gameType);

private:
	// QTcpSocket* pSocket;
	int socketDescriptor;
	Command* pCommand;
	CoridorLogic* gameCoridor;
	QuartoLogic* gameQuarto;
	// Player* player;

	void login(QString login);
	void switchCmd();
	// void playerList();
	// void sendString(QString message);
};

#endif // MYTHREAD_H
