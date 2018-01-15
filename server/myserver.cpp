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
			// WHERE THIS CONNECTS?
			/*for (const auto& j : lobbies)
				if (j->lobbyName == _lobby->lobbyName)
					j->connect(_player);*/
		}

		if (i->pPlayer->playerName == _lobby->host->playerName)
			emit i->connectToHostLobbySignal(i, hostLobby, _player, _connectFlag);
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

	/*if (!_connectFlag)
	{
		i->pLobby = _lobby;
		i->pLobby->disconnect(_player);
		for (const auto& j : lobbies)
			if (j->lobbyName == _lobby->lobbyName)
				j->disconnect(_player);
	}*/
	// qDebug() << _player->playerName;

	out << commandType;
	pCommand->operator<<(out);
	i->pSocket->write(arrBlock);
	i->pSocket->waitForBytesWritten();
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

void MyServer::sendQuitSlot(QString _reciever)
{
	for (const auto& i : threads)
		if (i->pPlayer->playerName == _reciever)
			i->sendQuit(_reciever);
}

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
	for (const auto& i : lobbies)
		if (i->lobbyName == _lobby->lobbyName)
		{
			for (const auto& j : threads)
				if (j->pPlayer->playerName == _lobby->host->playerName)
					j->sendMessage("Error: Lobby with that name already exists", true);
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
			emit i->changeGameTypeSignal(i, _gameType, _status);
		}
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

	qDebug() << "UpdateLobby Command Sent";
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

void MyServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << socketDescriptor << "Connecting...";
	threads.push_back(new MyThread(socketDescriptor, this));
	connect(threads.back(), SIGNAL(finished()), threads.back(), SLOT(deleteLater()));
	connect(threads.back(), SIGNAL(createPlayerSignal(Player*, MyThread*)), this, SLOT(createPlayer(Player*, MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deletePlayerSignal(MyThread*)), this, SLOT(deletePlayer(MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(createLobbySignal(Lobby*)), this, SLOT(createLobby(Lobby*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendGameTypesSignal(Player*, int, int)), this, SLOT(sendGameTypes(Player*, int, int)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(changeGameTypeSignal(MyThread*, int, int)), this, SLOT(changeGameType(MyThread*, int, int)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deleteLobbySignal(Lobby*)), this, SLOT(deleteLobby(Lobby*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(deleteGuestLobbySignal(Player*)), this, SLOT(deleteGuestLobby(Player*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendLobbiesListSignal(MyThread*)), this, SLOT(lobbiesList(MyThread*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(connectToLobbySignal(Lobby*, Player*, bool)), this, SLOT(sendConnectToLobby(Lobby*, Player*, bool)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(connectToHostLobbySignal(MyThread*, Lobby*, Player*, bool)), this, SLOT(sendConnectToLobbyHost(MyThread*, Lobby*, Player*, bool)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendRdySignal(Player*)), this, SLOT(sendRdy(Player*)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendMessageSignal(QString, bool, QString)), this, SLOT(sendMessageSlot(QString, bool, QString)));
	connect(threads.back(), SIGNAL(sendFirstPlayerSignal(QString, QString, GameType)), this, SLOT(sendFirstPlayerSlot(QString, QString, GameType)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(sendQuitSignal(QString)), this, SLOT(sendQuitSlot(QString)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(coridorSendQPointSignal(QPoint, bool, QString, bool)), this, SLOT(coridorSendQPointSlot(QPoint, bool, QString, bool)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(quartoSendQPointSignal(QPoint, int, QString)), this, SLOT(quartoSendQPointSlot(QPoint, int, QString)), Qt::DirectConnection);
	connect(threads.back(), SIGNAL(quartoSendCheckWinSignal(QString, bool)), this, SLOT(quartoSendCheckWinSlot(QString, bool)));
	threads.back()->start();
}
