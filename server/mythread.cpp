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

	sendString("Please, enter your name.");
	c = Command::AskLogin;

	qDebug() << socketDescriptor << "Client Connected";

	exec();
}

void MyThread::readyRead()
{
	QString data;
	QStringList myOptions;
	myOptions << "-help"
			  << "-players list"
			  << "-create lobby"
			  << "-lobbies list";

	QDataStream in(pSocket);
	in.setVersion(QDataStream::Qt_4_0);
	in >> data;

	QString lobbyName = "ErrorLobbyName";
	if (data.contains("-create lobby"))
	{
		lobbyName = data;
		lobbyName.remove(0, 14);
		data = "-create lobby";
		qDebug() << lobbyName;
	}

	qDebug() << data;

	if (c != Command::AskLogin)
	{
		switch (myOptions.indexOf(data))
		{
		case 0:
			c = Command::Help;
			break;
		case 1:
			c = Command::AskPlayers;
			break;
		case 2:
			c = Command::CreateLobby;
			break;
		case 3:
			c = Command::AskLobby;
			break;
		default:
			c = Command::WrongCommand;
			sendString("Wrong command '" + data + "'.");
			break;
		}
	}

	switch (c)
	{
	case AskLogin:
		login(data);
		break;
	case Help:
		help();
		break;
	case AskPlayers:
		emit sendPlayerList(this);
		break;
	case CreateLobby:
		emit createLobby(this, lobbyName);
		break;
	case AskLobbies:
		emit sendLobbiesList(this);
		break;
	case WrongCommand:
		break;
	}
}

void MyThread::login(QString login)
{
	player = new Player(login, socketDescriptor);
	sendString("Hello, " + login + "! Type -help to ask for available commands");
	c = Command::Help;
}

void MyThread::help()
{
	sendString("-help: ask for available commands\n-players list: ask for player list\n-create lobby 'LobbyName': ask for creating a lobby\n-lobbies list: ask for lobbies list\n");
}

/*void MyThread::playerList()
{
	while (!players.empty())
		sendString(players.back()->name + '/n');
	players.pop_back();
	sendString("only uuuuuuuuuuuuuuuuu");
}*/

void MyThread::disconnected()
{
	qDebug() << socketDescriptor << "Client Disconnected";
	pSocket->deleteLater();
	exit(0);
}

void MyThread::sendString(QString message)
{
	message += "\n";
	QByteArray buffer(message.toStdString().c_str());
	pSocket->write(buffer);
	pSocket->waitForBytesWritten();
}
