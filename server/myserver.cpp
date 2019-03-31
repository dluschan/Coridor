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

void MyServer::createPlayer(Player* _player, MyThread* _thread)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _player->playerName)
		{
			_thread->sendMessage("Error: Player with that name already logged in", true);
			return;
		}
	_thread->sendCreatePlayer(_player);
}

Lobby* MyServer::findLobby(Lobby* _lobby)
{
	for (const auto& j : lobbies)
	{
		qDebug() << j->lobbyName;
		if (j->lobbyName == _lobby->lobbyName)
			return j;
	}
	throw runtime_error("Error: This lobby doesn't exists anymore");
}

void MyServer::sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag)
{
	Lobby* hostLobby = findLobby(_lobby);

	if (hostLobby->connectedPlayersNumber >= hostLobby->maxPlayers && _connectFlag)
		throw runtime_error("Error: Too much players");

	if (_connectFlag)
		hostLobby->connect(_player);
	else
	{
		hostLobby->disconnect(_player);
		// if (_lobby->status != InGame)
		hostLobby->updateStatus(Unready);
	}

	for (const auto& i : threads)
	{
		if (i->pPlayer->playerName == _player->playerName && _connectFlag)
		{
			i->sendConnectToLobby(hostLobby, _player, _connectFlag);

			i->pLobby = hostLobby;

			// WHERE THIS CONNECTS?
			/*for (const auto& j : lobbies)
				if (j->lobbyName == _lobby->lobbyName)
					j->connect(_player);*/
		}

		if (i->pPlayer->playerName == hostLobby->host->playerName)
			i->sendConnectToLobby(hostLobby, _player, _connectFlag);
	}
}

void MyServer::sendRdy(Player* _host)
{
	/*for (const auto& j : lobbies)
	{
		if (i->host->playerName == _host->playerNam)
			if (j->status == Unready)
				j->updateStatus(Ready);
			else if (j->status == Ready)
				j->updateStatus(Unready);
	}*/
	for (const auto& j : lobbies)
		if (j->host->playerName == _host->playerName)
			if (j->status == Unready)
				j->updateStatus(Ready);
			else if (j->status == Ready)
				j->updateStatus(Unready);

	for (const auto& i : threads)
		if (i->pLobby->host->playerName == _host->playerName)
			i->sendRdy();
}

void MyServer::sendMessageSlot(QString _message, bool _errorFlag, QString _playerName)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _playerName)
			i->sendMessage(_message, _errorFlag);
}

void MyServer::sendFirstPlayerSlot(QString _firstPlayer, QString _guest, GameType _gameType)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _guest)
			i->sendFirstPlayer(_firstPlayer, _guest, _gameType);
}

/*void MyServer::sendQuitSlot(QString _reciever)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _reciever)
			i->sendQuit(_reciever);
}*/

void MyServer::coridorSendQPointSlot(QPoint _point, bool _move, QString _enemy, bool _horizontal)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _enemy)
			i->coridorSendQPoint(_point, _move, _enemy, _horizontal);
}

void MyServer::quartoSendQPointSlot(QPoint _point, int _figureId, QString _enemy)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _enemy)
			i->quartoSendQPoint(_point, _figureId, _enemy);
}

void MyServer::quartoSendCheckWinSlot(QString _enemy, bool _checkWin)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _enemy)
			i->quartoSendCheckWin(_enemy, _checkWin);
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

void MyServer::createLobby(Lobby* _lobby)
{
	if (_lobby->lobbyName == "")
	{
		for (const auto& j : threads)
		{
			if (j->pPlayer->playerName == _lobby->host->playerName)
				j->sendMessage("Error: Choose different name for the lobby", true);
		}
		return;
	}
	for (const auto& i : lobbies)
		if (i->lobbyName == _lobby->lobbyName)
		{
			for (const auto& j : threads)
			{
				if (j->pPlayer->playerName == _lobby->host->playerName)
					j->sendMessage("Error: Lobby with that name already exists", true);
			}
			return;
		}
	for (const auto& j : threads)
		if (j->pPlayer->playerName == _lobby->host->playerName)
			j->sendCreateLobby(_lobby);
	lobbies.push_back(_lobby);
}

void MyServer::sendGameTypes(Player* _player, int _gameType, int _status)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _player->playerName)
		{
			i->pLobby->update(_gameType, _status);
			for (const auto& j : lobbies)
				if (j->lobbyName == i->pLobby->lobbyName)
					j->update(_gameType, _status); // again?
			i->sendUpdateLobby(_gameType, _status);
		}
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
		if (i->pPlayer->playerName == _player->playerName)
			i->sendDeleteLobby(i->pLobby);
}

void MyServer::lobbiesList(MyThread* _thread)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_7);

	CommandType commandType = {CommandType::Type::SendLobbies};
	Command* pCommand = new SendLobbies(lobbies);

	out << commandType;
	pCommand->operator<<(out);
	_thread->write(arrBlock);
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << socketDescriptor << "Connecting...";
	threads.push_back(new MyThread(socketDescriptor, this));
	connect(threads.back(), SIGNAL(finished()), threads.back(), SLOT(deleteLater()));
	connect(threads.back(), SIGNAL(createPlayerSignal(Player*, MyThread*)), this, SLOT(createPlayer(Player*, MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deletePlayerSignal(MyThread*)), this, SLOT(deletePlayer(MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(createLobbySignal(Lobby*)), this, SLOT(createLobby(Lobby*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendGameTypesSignal(Player*, int, int)), this, SLOT(sendGameTypes(Player*, int, int)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deleteLobbyFromListSignal(Lobby*)), this, SLOT(deleteLobby(Lobby*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendDeleteLobbySignal(Player*)), this, SLOT(deleteGuestLobby(Player*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendLobbiesListSignal(MyThread*)), this, SLOT(lobbiesList(MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(connectToLobbySignal(Lobby*, Player*, bool)), this, SLOT(sendConnectToLobby(Lobby*, Player*, bool)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendRdySignal(Player*)), this, SLOT(sendRdy(Player*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendMessageSignal(QString, bool, QString)), this, SLOT(sendMessageSlot(QString, bool, QString)));
	connect(threads.back(), SIGNAL(sendFirstPlayerSignal(QString, QString, GameType)), this, SLOT(sendFirstPlayerSlot(QString, QString, GameType)), Qt::DirectConnection);
	// connect(threads.back(), SIGNAL(sendQuitSignal(QString)), this, SLOT(sendQuitSlot(QString)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(coridorSendQPointSignal(QPoint, bool, QString, bool)), this, SLOT(coridorSendQPointSlot(QPoint, bool, QString, bool)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(quartoSendQPointSignal(QPoint, int, QString)), this, SLOT(quartoSendQPointSlot(QPoint, int, QString)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(quartoSendCheckWinSignal(QString, bool)), this, SLOT(quartoSendCheckWinSlot(QString, bool)));
	threads.back()->start();
}
