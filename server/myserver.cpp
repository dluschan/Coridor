#include "myserver.h"

MyServer::MyServer(QObject* parent)
	: QTcpServer(parent)
{
}

void MyServer::StartServer()
{
	if (!this->listen(QHostAddress::Any, 5555))
	{
		qDebug() << "Couldn't start the server";
	}
	else
	{
		qDebug() << "Listening...";
	}
}

void MyServer::sendString(QString message, MyThread* thread)
{
	thread->sendString(message);
}

/*void MyServer::playerList(MyThread* thread)
{
	QString message;
	for (const auto& i : players)
		message += i->pPlayer->playerName + " - status" + '\n';
	sendString(message, thread);
}*/

void MyServer::createLobby(Lobby* lobby)
{
	lobbies.push_back(lobby);
}

/*void MyServer::lobbyList(MyThread* thread)
{
	QString message = "";
	for (const auto& i : lobbies)
		message += i->lobbyName + " - Host: " + thread->pPlayer->playerName + " - status" + '\n';
	if (message != "")
		sendString(message, thread);
	else
		sendString("No lobbies", thread);
}*/

void MyServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << socketDescriptor << "Connecting...";
	players.push_back(new MyThread(socketDescriptor, this));
	connect(players.back(), SIGNAL(finished()), players.back(), SLOT(deleteLater()));
	// connect(players.back(), SIGNAL(sendPlayerList(MyThread*)), this, SLOT(playerList(MyThread*)), Qt::DirectConnection);
	connect(players.back(), SIGNAL(createLobbySignal(MyThread*, QString, int)), this, SLOT(createLobby(MyThread*, QString, int)));
	// connect(players.back(), SIGNAL(sendLobbiesList(MyThread*)), this, SLOT(lobbyList(MyThread*)));
	players.back()->start();

	/* MyThread* thread = new MyThread(socketDescriptor, this);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start(); */
}
