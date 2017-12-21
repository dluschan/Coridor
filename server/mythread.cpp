#include "mythread.h"
#include "../common/command.h"

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

	qDebug() << socketDescriptor << "Client Connected";

	exec();
}

void MyThread::readyRead()
{
	QByteArray arrBlock = pSocket->readAll();
	qDebug() << arrBlock;
	QDataStream in(&arrBlock, QIODevice::ReadOnly);
	in.setVersion(QDataStream::Qt_4_0);

	CommandFactory factory;
	Command* pCommand = factory.create(in);
	pCommand->execute();
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
