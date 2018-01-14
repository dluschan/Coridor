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
	pPlayer = new Player();
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

void MyThread::sendCreatePlayer(Player* _player)
{
	pPlayer = _player;
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::AskLogin};
	Command* pCommand = new Login(_player->playerName);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);

	qDebug() << "Login Command Sent";
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

	qDebug() << "UpdateLobby Command Sent";
}

void MyThread::sendFirstPlayer(QString _firstPlayer, QString _guest, GameType _gameType)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendFirstPlayer};
	Command* pCommand = new SendFirstPlayer(_firstPlayer, _guest, _gameType);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);

	qDebug() << "SendFirstPlayer Command Sent";
}

void MyThread::sendQuit(QString _reciever)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendQuit};
	Command* pCommand = new SendQuit(_reciever);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);

	qDebug() << "SendQuit Command Sent";
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

void MyThread::quartoSendQPoint(QPoint point, int figureId, QString enemy)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::QuartoSendQPoint};
	Command* pCommand = new QuartoSendQPoint(point, figureId, enemy);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "CoridorSendQPoint Command Sent";
}

void MyThread::quartoSendCheckWin(QString enemy, bool checkWin)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::QuartoSendCheckWin};
	Command* pCommand = new QuartoSendCheckWin(enemy, checkWin);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "QuartoSendCheckWin Command Sent";
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

void MyThread::sendCreateLobby(Lobby* _lobby)
{
	pLobby = _lobby;
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::CreateLobby};
	Command* pCommand = new CreateLobby(pLobby);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);

	qDebug() << "CreateLobby Command Sent";
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
		emit createPlayerSignal(pLogin->player, this);
		// pPlayer = pLogin->player;
		// pPlayer->setID(socketDescriptor);
	}
	else if (CreateLobby* pCreateLobby = dynamic_cast<CreateLobby*>(pCommand))
	{
		emit createLobbySignal(pCreateLobby->lobby);
	}
	else if (UpdateLobby* pUpdateLobby = dynamic_cast<UpdateLobby*>(pCommand))
	{
		// emit changeGameTypeSignal(this, pUpdateLobby->gameType, pUpdateLobby->status);
		pLobby->update(pUpdateLobby->gameType, pUpdateLobby->status);
		for (const auto& i : pUpdateLobby->connectedPlayers)
			emit sendGameTypesSignal(i, pUpdateLobby->gameType, pUpdateLobby->status);
	}
	else if (DeleteLobby* pDeleteLobby = dynamic_cast<DeleteLobby*>(pCommand))
	{
		if (pDeleteLobby->lobby->gameType != WrongGameType)
		{
			for (const auto& i : pDeleteLobby->lobby->connectedPlayers)
			{
				emit deleteGuestLobbySignal(i);
			}
			emit deleteLobbySignal(pDeleteLobby->lobby);
			pLobby = new Lobby();
		}
		else
		{
			emit deleteLobbySignal(pDeleteLobby->lobby);
			pLobby = new Lobby();
		}
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
	// dont need it now
	/*else if (SendMessage* pSendMessage = dynamic_cast<SendMessage*>(pCommand))
	{
		if (pSendMessage->errorFlag)
			emit sendMessageSignal(pSendMessage->message, pSendMessage->errorFlag, pSendMessage->playerName);
	}*/
	else if (SendFirstPlayer* pSendFirstPlayer = dynamic_cast<SendFirstPlayer*>(pCommand))
	{
		emit sendFirstPlayerSignal(pSendFirstPlayer->firstPlayer, pSendFirstPlayer->guest, (GameType)pSendFirstPlayer->gameType);
	}
	else if (SendQuit* pSendQuit = dynamic_cast<SendQuit*>(pCommand))
	{
		emit sendQuitSignal(pSendQuit->reciever);
	}
	else if (CoridorSendQPoint* pCoridorSendQPoint = dynamic_cast<CoridorSendQPoint*>(pCommand))
	{
		emit coridorSendQPointSignal(pCoridorSendQPoint->point, pCoridorSendQPoint->move, pCoridorSendQPoint->enemy, pCoridorSendQPoint->horizontal);
	}
	else if (QuartoSendQPoint* pQuartoSendQPoint = dynamic_cast<QuartoSendQPoint*>(pCommand))
	{
		emit quartoSendQPointSignal(pQuartoSendQPoint->point, pQuartoSendQPoint->figureId, pQuartoSendQPoint->enemy);
	}
	else if (QuartoSendCheckWin* pQuartoSendCheckWin = dynamic_cast<QuartoSendCheckWin*>(pCommand))
	{
		emit quartoSendCheckWinSignal(pQuartoSendCheckWin->enemy, pQuartoSendCheckWin->checkWin);
	}
}
