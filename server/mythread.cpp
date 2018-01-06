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
		emit error(pSocket->error());
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

	emit sendPlayerList(this);
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
	emit deletePlayerSignal(this);
	emit deleteLobbySignal(pLobby);
	qDebug() << socketDescriptor << "Client Disconnected";
	pSocket->deleteLater();
	exit(0);
}

void MyThread::write(QByteArray buffer)
{
	pSocket->write(buffer);
	pSocket->waitForBytesWritten();
}

void MyThread::sendString(QString message)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendString};
	Command* pCommand = new SendString(message);

	out << commandType;
	pCommand->operator<<(out);
	write(arrBlock);
}

Lobby* MyThread::createLobby(QString lobbyName, int gameType)
{
	return new Lobby(lobbyName, pPlayer->playerName, gameType);
}

void MyThread::switchCmd()
{
	if (Login* pLogin = dynamic_cast<Login*>(pCommand))
	{
		pPlayer = pLogin->player;
		pPlayer->setID(socketDescriptor);
	}
	else if (CreateLobby* pCreateLobby = dynamic_cast<CreateLobby*>(pCommand))
	{
		pLobby = pCreateLobby->lobby;
		emit createLobbySignal(pCreateLobby->lobby);
	}
	else if (ChangeGameType* pChangeGameType = dynamic_cast<ChangeGameType*>(pCommand))
	{
		pLobby->update(pChangeGameType->gameType);
		emit changeGameTypeSignal(this, pChangeGameType->gameType);
	}
	else if (DeleteLobby* pDeleteLobby = dynamic_cast<DeleteLobby*>(pCommand))
	{
		pLobby = new Lobby();
		emit deleteLobbySignal(pLobby);
	}
	else if (AskLobbies* pAskLobbies = dynamic_cast<AskLobbies*>(pCommand))
	{
		emit sendLobbiesList(this);
	}
}
