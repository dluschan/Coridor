#include "mythread.h"

MyThread::MyThread(int ID, QObject* parent)
	: QThread(parent)
{
	this->socketDescriptor = ID;
}

void MyThread::run()
{
	qDebug() << socketDescriptor << "Starting thread";
	pSocket = new QTcpSocket();
	if (!pSocket->setSocketDescriptor(this->socketDescriptor))
	{
		emit errorSignal(pSocket->error());
		return;
	}

	connect(pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

	qDebug() << socketDescriptor << "Client Connected";

	exec();
}

void MyThread::readyRead()
{
	CommandFactory factory;
	QDataStream in(pSocket);
	in.setVersion(QDataStream::Qt_5_9);
	pCommand = factory.create(in);
	pCommand->execute();
	switchCmd();

	// emit sendPlayerListSignal(this);
}

/*
void MyThread::login(QString login)
{
	player = new Player(login, socketDescriptor);
	sendString("Hello, " + login + "! Type -help to ask for available CommandTypes");
	c = CommandType::Help;
}

void MyThread::help()
{
	sendString("-help: ask for available CommandTypes\n-players list: ask for player list\n-create lobby 'LobbyName': ask for creating a lobby\n-lobbies list: ask for lobbies list\n");
}

void MyThread::playerList()
{
	while (!players.empty())
		sendString(players.back()->name + '/n');
	players.pop_back();
	sendString("only uuuuuuuuuuuuuuuuu");
}*/

void MyThread::disconnected()
{
	emit deleteLobbySignal(pLobby);
	emit deletePlayerSignal(this);
	qDebug() << socketDescriptor << "Client Disconnected";
	pSocket->deleteLater();
	exit(0);
}

void MyThread::changeGameType(int _gameType)
{
	pLobby->gameType = (GameType)_gameType;
}

void MyThread::write(QByteArray buffer)
{
	pSocket->write(buffer);
	pSocket->waitForBytesWritten();
}

void MyThread::sendRdy()
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendRdy};
	Command* pCommand = new SendRdy(pLobby->host);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);

	qDebug() << "SendRdy Command Sent";
}

void MyThread::sendStart()
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::UpdateLobby};
	Command* pCommand = new UpdateLobby(pLobby->gameType, pLobby->status, pLobby->connectedPlayers);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);
}

void MyThread::sendFirstPlayer(QString _firstPlayer, QString _guest)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendFirstPlayer};
	Command* pCommand = new SendFirstPlayer(_firstPlayer, _guest);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);
}

void MyThread::coridorSendQPoint(QPoint point, bool move, QString enemy, bool horizontal)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::CoridorSendQPoint};
	Command* pCommand = new CoridorSendQPoint(point, move, enemy, horizontal);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "CoridorSendQPoint Command Sent";
}

void MyThread::sendMessage(QString message, bool error)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendMessage};
	Command* pCommand = new SendMessage(message, error);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);
}

Lobby* MyThread::createLobby(QString lobbyName, int gameType)
{
	return new Lobby(lobbyName, pPlayer->playerName, gameType);
}

void* MyThread::deleteGuestLobby(Lobby* lobby)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::DeleteLobby};
	Command* pCommand = new DeleteLobby(lobby);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);

	qDebug() << "DeleteLobby Command Sent";
	pLobby = new Lobby();
}

void MyThread::switchCmd()
{
	if (Login* pLogin = dynamic_cast<Login*>(pCommand))
	{
		pPlayer = pLogin->player;
		// pPlayer->setID(socketDescriptor);
	}
	else if (CreateLobby* pCreateLobby = dynamic_cast<CreateLobby*>(pCommand))
	{
		emit createLobbySignal(pCreateLobby->lobby);
		pLobby = pCreateLobby->lobby;
	}
	else if (UpdateLobby* pUpdateLobby = dynamic_cast<UpdateLobby*>(pCommand))
	{
		// emit changeGameTypeSignal(this, pChangeGameType->gameType, pChangeGameType->status);
		pLobby->update(pUpdateLobby->gameType, pUpdateLobby->status);
		for (const auto& i : pUpdateLobby->connectedPlayers)
			emit sendGameTypesSignal(i, pUpdateLobby->gameType, pUpdateLobby->status);
	}
	else if (DeleteLobby* pDeleteLobby = dynamic_cast<DeleteLobby*>(pCommand))
	{
		for (const auto& i : pDeleteLobby->lobby->connectedPlayers)
		{
			emit deleteGuestLobbySignal(i);
		}
		emit deleteLobbySignal(pDeleteLobby->lobby);
		pLobby = new Lobby();
	}
	else if (AskLobbies* pAskLobbies = dynamic_cast<AskLobbies*>(pCommand))
	{
		emit sendLobbiesListSignal(this);
	}
	else if (ConnectToLobby* pConnectToLobby = dynamic_cast<ConnectToLobby*>(pCommand))
	{
		if (pConnectToLobby->connectFlag)
		{
			if (pConnectToLobby->lobby->connectedPlayersNumber < pConnectToLobby->lobby->maxPlayers)
				try
				{
					emit connectToLobbySignal(pConnectToLobby->lobby, pConnectToLobby->player, true);
				}
				catch (const exception& e)
				{
					emit sendMessage(tr(e.what()), true);
					qDebug() << tr(e.what());
				}
			else
				sendMessage("Error: Too much players", true);
		}
		else
		{
			emit connectToLobbySignal(pConnectToLobby->lobby, pConnectToLobby->player, false);
			pLobby = new Lobby();
		}
	}
	else if (SendRdy* pSendRdy = dynamic_cast<SendRdy*>(pCommand))
	{
		emit sendRdySignal(pSendRdy->host);
		if (pLobby->status == Unready)
			pLobby->updateStatus(Ready);
		else if (pLobby->status == Ready)
			pLobby->updateStatus(Unready);
	}
	else if (SendFirstPlayer* pSendFirstPlayer = dynamic_cast<SendFirstPlayer*>(pCommand))
	{
		emit sendFirstPlayerSignal(pSendFirstPlayer->firstPlayer, pSendFirstPlayer->guest);
	}
	else if (CoridorSendQPoint* pCoridorSendQPoint = dynamic_cast<CoridorSendQPoint*>(pCommand))
	{
		emit coridorSendQPointSignal(pCoridorSendQPoint->point, pCoridorSendQPoint->move, pCoridorSendQPoint->enemy, pCoridorSendQPoint->horizontal);
	}
}
