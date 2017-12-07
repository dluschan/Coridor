#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "lobby.h"
//#include "player.h"
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
	Player* player;
	Lobby* lobby;
	void run();
	void sendString(QString message);

signals:
	void error(QTcpSocket::SocketError socketError);
	void sendPlayerList(MyThread* thread);
	void createLobby(MyThread* thread, QString lobbyName);
	void sendLobbiesList(MyThread* socket);

public slots:
	void readyRead();
	void disconnected();

private:
	// QTcpSocket* pSocket;
	int socketDescriptor;
	// Player* player;

	void login(QString login);
	void help();
	// void playerList();
	// void sendString(QString message);
};

#endif // MYTHREAD_H
