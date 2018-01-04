#include "mainwindow.h"
#include "../common/command.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	resize(400, 300);
	setMinimumWidth(400);
	setMinimumHeight(300);
	setWindowTitle("Client");

	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	pSocket = new QTcpSocket(this);
	connect(pSocket, SIGNAL(readyRead()), this, SLOT(sockReady()));
	connect(pSocket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

	connectLayout = new QVBoxLayout(centralWidget);
	connectBtn = new QPushButton("Connect to server");
	loginEdit = new QLineEdit();
	passwordEdit = new QLineEdit();
	connectLayout->addWidget(loginEdit);
	connectLayout->addWidget(passwordEdit);
	connectLayout->addWidget(connectBtn);
	connect(connectBtn, SIGNAL(clicked()), this, SLOT(connectToTheServer()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectToTheServer()
{
	pSocket->connectToHost("127.0.0.1", 5555);

	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::AskLogin};
	Command* pCommand = new Login(loginEdit->text());

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "Command Sent";

	switchToMain();
}

void MainWindow::createLobbyDialog()
{
	CreateLobbyDialog* dialog = new CreateLobbyDialog();
	connect(dialog, SIGNAL(clicked2(QString, QString, int)), this, SLOT(createLobby(QString, QString, int)));
	// (dialog->lobbyNameEdit->text(), dialog->hostLoginEdit->text(), dialog->gameTypeEdit->currentIndex())
}

void MainWindow::switchToLoginIn()
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	connectLayout = new QVBoxLayout(centralWidget);
	connectBtn = new QPushButton("Connect to server");
	loginEdit = new QLineEdit();
	passwordEdit = new QLineEdit();
	connectLayout->addWidget(loginEdit);
	connectLayout->addWidget(passwordEdit);
	connectLayout->addWidget(connectBtn);
	connect(connectBtn, SIGNAL(clicked()), this, SLOT(connectToTheServer()));
}

void MainWindow::switchToMain()
{
	// delete centralWidget->layout();
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);
	mainLayout = new QVBoxLayout(centralWidget);
	createLobbyBtn = new QPushButton("Create Lobby");
	showLobbiesBtn = new QPushButton("Connect to Lobby");
	disconnectBtn = new QPushButton("Log out");
	mainLayout->addWidget(createLobbyBtn);
	mainLayout->addWidget(showLobbiesBtn);
	mainLayout->addWidget(disconnectBtn);
	connect(disconnectBtn, SIGNAL(clicked()), this, SLOT(sockDisc()));
	connect(createLobbyBtn, SIGNAL(clicked()), this, SLOT(createLobbyDialog()));
}

void MainWindow::sockDisc()
{
	pSocket->deleteLater();
	switchToLoginIn();
}

void MainWindow::createLobby(QString _lobbyName, QString _hostLogin, int _gameType)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::CreateLobby};
	Command* pCommand = new CreateLobby(_lobbyName, _hostLogin, _gameType);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();
}

void MainWindow::sockReady()
{
	if (pSocket->waitForConnected(500))
	{
		pSocket->waitForReadyRead(500);
		data = pSocket->readAll();
		qDebug() << data;
	}
}

/*void MainWindow::sendMessage()
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_10);

	CommandType commandType = {CommandType::Type::AskLogin};
	Command* pCommand = new Login(ui->lineEdit->text());

	out << commandType;
	pCommand->operator<<(out);

	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	// lineEdit->setText("");

	qDebug() << "Message Sent";
}*/
