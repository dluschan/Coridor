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

void MyServer::sendString(QString _message, MyThread* _thread)
{
	_thread->sendString(_message);
}

void MyServer::sendConnectToLobby(Lobby* _lobby, Player* _player)
{
	for (const auto& i : threads)
	{
		if (i->pPlayer->playerName == _player->playerName)
		{
			QByteArray arrBlock;
			QDataStream out(&arrBlock, QIODevice::WriteOnly);
			out.setVersion(QDataStream::Qt_5_9);

			CommandType commandType = {CommandType::Type::ConnectToLobby};
			Command* pCommand = new ConnectToLobby(_lobby, _player);

			// qDebug() << i->pPlayer->playerName << _player->playerName << _lobby->host->playerName;

			i->pLobby = _lobby;
			i->pLobby->connect(_player);
			for (const auto& j : lobbies)
				if (j->lobbyName == _lobby->lobbyName)
					j->connect(_player);

			out << commandType;
			pCommand->operator<<(out);
			i->pSocket->write(arrBlock);
			i->pSocket->waitForBytesWritten();
		}
		// qDebug() << i->pPlayer->playerName << _player->playerName << _lobby->host->playerName;
		if (i->pPlayer->playerName == _lobby->host->playerName)
			emit i->connectToHostLobbySignal(i, _lobby, _player);
	}

	qDebug() << "ConnectToLobby Command Sent";
}

void MyServer::sendConnectToLobbyHost(MyThread* i, Lobby* _lobby, Player* _player)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::ConnectToLobby};
	Command* pCommand = new ConnectToLobby(_lobby, _player);

	/*i->pLobby = _lobby;
	i->pLobby->connect(_player);
	for (const auto& j : lobbies)
		if (j->lobbyName == _lobby->lobbyName)
			j->connect(_player);*/

	qDebug() << _player->playerName;

	out << commandType;
	pCommand->operator<<(out);
	i->pSocket->write(arrBlock);
	i->pSocket->waitForBytesWritten();
}

void MyServer::deletePlayer(MyThread* _thread)
{
	threads.remove(_thread);
}

/*void MyServer::playerList(MyThread* thread)
{
	QString message;
	for (const auto& i : players)
		message += i->pPlayer->playerName + " - status" + '\n';
	sendString(message, thread);
}*/

void MyServer::createLobby(Lobby* _lobby)
{
	lobbies.push_back(_lobby);
}

void MyServer::changeGameType(MyThread* _thread, int _gameType)
{
	for (const auto& i : lobbies)
		if (i == _thread->pLobby)
			i->updateGameType(_gameType);
}

void MyServer::sendGameTypes(Player* _player, int _gameType)
{
	// qDebug() << "HERE";
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _player->playerName)
			emit i->sendGameTypeSignal(i, _gameType);
}

void MyServer::sendGameType(MyThread* _thread, int _gameType)
{
	_thread->pLobby->updateGameType(_gameType);
	for (const auto& i : lobbies)
		if (i == _thread->pLobby)
			i->updateGameType(_gameType);

	_thread->pLobby->updateGameType(_gameType);
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::ChangeGameType};
	Command* pCommand = new ChangeGameType(_gameType, _thread->pLobby->connectedPlayers);

	out << commandType;
	pCommand->operator<<(out);
	_thread->pSocket->write(arrBlock);
	_thread->pSocket->waitForBytesWritten();

	qDebug() << "ChangeGameType Command Sent";

	// qDebug() << _thread->pLobby->gameTypeStr;
}

void MyServer::deleteLobby(Lobby* _lobby)
{
	lobbies.remove(_lobby);
}

void MyServer::lobbiesList(MyThread* _thread)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendLobbies};
	Command* pCommand = new SendLobbies(lobbies);

	out << commandType;
	pCommand->operator<<(out);
	_thread->write(arrBlock);
}

void MyServer::connectToLobby(Lobby* _lobby, Player* _player)
{
	// bool flag = true;
	for (const auto& i : lobbies)
		if (i == _lobby && _lobby->maxPlayers != _lobby->connectedPlayersNumber)
		{
			i->connect(_player);

			sendConnectToLobby(i, _player);
			return;
			// flag = false;
		}
	// SendError
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
	threads.push_back(new MyThread(socketDescriptor, this));
	connect(threads.back(), SIGNAL(finished()), threads.back(), SLOT(deleteLater()));
	// connect(players.back(), SIGNAL(sendPlayerList(MyThread*)), this, SLOT(playerList(MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(createLobbySignal(Lobby*)), this, SLOT(createLobby(Lobby*)));
	connect(threads.back(), SIGNAL(changeGameTypeSignal(MyThread*, int)), this, SLOT(changeGameType(MyThread*, int)), Qt::DirectConnection);
	// qRegisterMetaType(std::list<Player*>);
	connect(threads.back(), SIGNAL(sendGameTypesSignal(Player*, int)), this, SLOT(sendGameTypes(Player*, int)));
	connect(threads.back(), SIGNAL(sendGameTypeSignal(MyThread*, int)), this, SLOT(sendGameType(MyThread*, int)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deleteLobbySignal(Lobby*)), this, SLOT(deleteLobby(Lobby*)));
	connect(threads.back(), SIGNAL(sendLobbiesListSignal(MyThread*)), this, SLOT(lobbiesList(MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(connectToLobbySignal(Lobby*, Player*)), this, SLOT(sendConnectToLobby(Lobby*, Player*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(connectToHostLobbySignal(MyThread*, Lobby*, Player*)), this, SLOT(sendConnectToLobbyHost(MyThread*, Lobby*, Player*)), Qt::DirectConnection);
	threads.back()->start();

	/* MyThread* thread = new MyThread(socketDescriptor, this);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start(); */
}
