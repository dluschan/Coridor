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

void MyServer::deleteLobby(Lobby* lobby)
{
	lobbies.remove(lobby);
}

void MyServer::lobbiesList(MyThread* thread)
{
	qDebug() << "OK";
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendLobbies};
	Command* pCommand = new SendLobbies(lobbies);

	out << commandType;
	pCommand->operator<<(out);
	thread->write(arrBlock);
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
	connect(players.back(), SIGNAL(createLobbySignal(Lobby*)), this, SLOT(createLobby(Lobby*)));
	connect(players.back(), SIGNAL(deleteLobbySignal(Lobby*)), this, SLOT(deleteLobby(Lobby*)));
	connect(players.back(), SIGNAL(sendLobbiesList(MyThread*)), this, SLOT(lobbiesList(MyThread*)), Qt::DirectConnection);
	players.back()->start();

	/* MyThread* thread = new MyThread(socketDescriptor, this);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start(); */
}
