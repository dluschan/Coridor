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
	_thread->sendMessage(_message, false);
}

void MyServer::sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag)
{
	Lobby* hostLobby = new Lobby();

	for (const auto& j : lobbies)
	{
		qDebug() << j->lobbyName;
		if (j->lobbyName == _lobby->lobbyName)
			hostLobby = j;
	}

	if (hostLobby->lobbyName == "LobbyName" && hostLobby->host->playerName == "HostName" && hostLobby->gameType == WrongGameType)
	{
		throw runtime_error("Error: This lobby doesn't exists anymore");
	}

	for (const auto& i : threads)
	{
		if (i->pPlayer->playerName == _player->playerName && _connectFlag)
		{
			QByteArray arrBlock;
			QDataStream out(&arrBlock, QIODevice::WriteOnly);
			out.setVersion(QDataStream::Qt_5_9);

			CommandType commandType = {CommandType::Type::ConnectToLobby};
			qDebug() << hostLobby->host->playerName;
			Command* pCommand = new ConnectToLobby(hostLobby, _player, _connectFlag);

			out << commandType;
			pCommand->operator<<(out);
			i->pSocket->write(arrBlock);
			i->pSocket->waitForBytesWritten();

			i->pLobby = _lobby;
			i->pLobby->connect(_player);
			for (const auto& j : lobbies)
				if (j->lobbyName == _lobby->lobbyName)
					j->connect(_player);
		}

		if (i->pPlayer->playerName == _lobby->host->playerName)
			if (_connectFlag)
				emit i->connectToHostLobbySignal(i, hostLobby, _player, _connectFlag);
			else
				emit i->connectToHostLobbySignal(i, _lobby, _player, _connectFlag);
	}

	qDebug() << "ConnectToLobby Command Sent";
}

void MyServer::sendConnectToLobbyHost(MyThread* i, Lobby* _lobby, Player* _player, bool _connectFlag)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::ConnectToLobby};
	Command* pCommand = new ConnectToLobby(_lobby, _player, _connectFlag);

	if (!_connectFlag)
	{
		i->pLobby = _lobby;
		i->pLobby->disconnect(_player);
		for (const auto& j : lobbies)
			if (j->lobbyName == _lobby->lobbyName)
				j->disconnect(_player);
	}
	// qDebug() << _player->playerName;

	out << commandType;
	pCommand->operator<<(out);
	i->pSocket->write(arrBlock);
	i->pSocket->waitForBytesWritten();
}

void MyServer::sendRdy(Player* _host)
{
	for (const auto& i : threads)
	{
		if (i->pPlayer->playerName == _host->playerName)
			for (const auto& j : lobbies)
				if (j->status == Unready)
					j->updateStatus(Ready);
				else if (j->status == Ready)
					j->updateStatus(Unready);
		if (i->pLobby->host->playerName == _host->playerName)
			i->sendRdy();
	}
}

/*void MyServer::sendStart(Player* _connectedPlayer)
{
	for (const auto& i : threads)
	{
		if (i->pPlayer->playerName == _connectedPlayer->playerName)
		{
			if (i->pLobby->status == Ready)
				i->pLobby->updateStatus(InGame);
			i->sendStart();
			for (const auto& j : lobbies)
				if (j->status == Ready)
					j->updateStatus(InGame);
		}
	}
}*/

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

/*void MyServer::changeGameType(MyThread* _thread, int _gameType)
{
	for (const auto& i : lobbies)
		if (i->lobbyName == _thread->pLobby->lobbyName)
		{
			i->updateGameType(_gameType);
			qDebug() << i->gameTypeStr;
		}
}*/

void MyServer::sendGameTypes(Player* _player, int _gameType, int _status)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _player->playerName)
			emit i->changeGameTypeSignal(i, _gameType, _status);
}

void MyServer::changeGameType(MyThread* _thread, int _gameType, int _status)
{
	_thread->pLobby->update(_gameType, _status);
	for (const auto& i : lobbies)
		if (i->lobbyName == _thread->pLobby->lobbyName)
			i->update(_gameType, _status); // again?

	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::UpdateLobby};
	Command* pCommand = new UpdateLobby(_gameType, _status, _thread->pLobby->connectedPlayers);

	out << commandType;
	pCommand->operator<<(out);
	_thread->pSocket->write(arrBlock);
	_thread->pSocket->waitForBytesWritten();

	qDebug() << "ChangeGameType Command Sent";

	// qDebug() << _thread->pLobby->gameTypeStr;
}

void MyServer::deleteLobby(Lobby* _lobby)
{
	for (const auto& i : lobbies)
		if (i->lobbyName == _lobby->lobbyName)
			lobbies.remove(i);
}

void MyServer::deleteGuestLobby(Player* _player)
{
	for (const auto& i : threads)
	{
		// qDebug() << i->pPlayer->playerName << i->pLobby->lobbyName; //<< _player->playerName;
		if (i->pPlayer->playerName == _player->playerName)
		{
			i->deleteGuestLobby(i->pLobby);
			i->pLobby = new Lobby();
		}
	}
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

/*void MyServer::connectToLobby(Lobby* _lobby, Player* _player)
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
}*/

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
	connect(threads.back(), SIGNAL(createLobbySignal(Lobby*)), this, SLOT(createLobby(Lobby*)), Qt::DirectConnection);
	// connect(threads.back(), SIGNAL(changeGameTypeSignal(MyThread*, int)), this, SLOT(changeGameType(MyThread*, int)), Qt::DirectConnection);
	// qRegisterMetaType(std::list<Player*>);
	connect(threads.back(), SIGNAL(sendGameTypesSignal(Player*, int, int)), this, SLOT(sendGameTypes(Player*, int, int)));
	connect(threads.back(), SIGNAL(changeGameTypeSignal(MyThread*, int, int)), this, SLOT(changeGameType(MyThread*, int, int)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deleteLobbySignal(Lobby*)), this, SLOT(deleteLobby(Lobby*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deleteGuestLobbySignal(Player*)), this, SLOT(deleteGuestLobby(Player*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendLobbiesListSignal(MyThread*)), this, SLOT(lobbiesList(MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(connectToLobbySignal(Lobby*, Player*, bool)), this, SLOT(sendConnectToLobby(Lobby*, Player*, bool)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(connectToHostLobbySignal(MyThread*, Lobby*, Player*, bool)), this, SLOT(sendConnectToLobbyHost(MyThread*, Lobby*, Player*, bool)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendRdySignal(Player*)), this, SLOT(sendRdy(Player*)), Qt::DirectConnection);
	// connect(threads.back(), SIGNAL(sendStartSignal(Player*)), this, SLOT(sendStart(Player*)), Qt::DirectConnection);
	threads.back()->start();

	/* MyThread* thread = new MyThread(socketDescriptor, this);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start(); */
}
