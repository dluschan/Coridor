#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "../common/command.h"
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QtNetwork>
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
	void sendString(QString message);
	Lobby* createLobby(QString lobbyName, int gameType);
	void write(QByteArray buffer);
	void sendRdy();
	// Player* createPlayer(QString playerName);

signals:
	void deletePlayerSignal(MyThread* thread);
	void errorSignal(QTcpSocket::SocketError socketError);
	void sendPlayerListSignal(MyThread* thread);
	void createLobbySignal(Lobby* pLobby);
	void changeGameTypeSignal(MyThread* thread, int gameType);
	void sendGameTypesSignal(Player* players, int gameType);
	void sendGameTypeSignal(MyThread* thread, int gameType);
	void deleteLobbySignal(Lobby* pLobby);
	void connectToLobbySignal(Lobby* pLobby, Player* player);
	void connectToHostLobbySignal(MyThread* thread, Lobby* pLobby, Player* player);
	void sendLobbiesListSignal(MyThread* thread);
	void sendRdySignal(Player* host);

public slots:
	void readyRead();
	void disconnected();
	void changeGameType(int _gameType);

private:
	// QTcpSocket* pSocket;
	int socketDescriptor;
	Command* pCommand;
	// Player* player;

	void login(QString login);
	void switchCmd();
	// void playerList();
	// void sendString(QString message);
};

#endif // MYTHREAD_H
