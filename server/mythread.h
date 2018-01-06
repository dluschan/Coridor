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
	Lobby* pLobby;
	void run();
	void sendString(QString message);
	Lobby* createLobby(QString lobbyName, int gameType);
	void write(QByteArray buffer);
	// Player* createPlayer(QString playerName);

signals:
	void error(QTcpSocket::SocketError socketError);
	void sendPlayerList(MyThread* thread);
	void createLobbySignal(Lobby* pLobby);
	void sendLobbiesList(MyThread* thread);

public slots:
	void readyRead();
	void disconnected();

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
