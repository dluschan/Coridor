#include "mainwindow.h"

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

	connectLayout = new QGridLayout(centralWidget);
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
	Command* pCommand = new SendString(loginEdit->text());

	player = new Player(loginEdit->text());
	// qDebug() << int(pSocket->socketDescriptor());
	// player->setID(int(pSocket->socketDescriptor()));

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "Login Command Sent";

	switchToMain();
}

void MainWindow::createLobbyDialog()
{
	CreateLobbyDialog* dialog = new CreateLobbyDialog(player->playerName);
	connect(dialog, SIGNAL(clicked(QString, QString, int)), this, SLOT(createLobby(QString, QString, int)));
	// (dialog->lobbyNameEdit->text(), dialog->hostLoginEdit->text(), dialog->gameTypeEdit->currentIndex())
}

void MainWindow::connectToLobby(QTreeWidgetItem* item, int column)
{
	qDebug() << "Connect to lobby";
}

void MainWindow::leaveLobby()
{
	deleteLobby(lobby->lobbyName);
	switchToMain();
}

void MainWindow::leaveLobbiesList()
{
	switchToMain();
}

void MainWindow::switchToLoginIn()
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	connectLayout = new QGridLayout(centralWidget);
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
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	mainLayout = new QGridLayout(centralWidget);
	createLobbyBtn = new QPushButton("Create Lobby");
	showLobbiesBtn = new QPushButton("Connect to Lobby");
	disconnectBtn = new QPushButton("Log out");

	mainLayout->addWidget(createLobbyBtn);
	mainLayout->addWidget(showLobbiesBtn);
	mainLayout->addWidget(disconnectBtn);

	connect(disconnectBtn, SIGNAL(clicked()), this, SLOT(sockDisc()));
	connect(showLobbiesBtn, SIGNAL(clicked()), this, SLOT(askLobbies()));
	connect(createLobbyBtn, SIGNAL(clicked()), this, SLOT(createLobbyDialog()));
}

void MainWindow::switchToLobby(int _gameType)
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	lobbyLayout = new QGridLayout(centralWidget);

	connectedPlayersList = new QListWidget();
	QListWidgetItem* player1 = new QListWidgetItem(player->playerName, connectedPlayersList);
	player1->setCheckState(Qt::Unchecked);
	QListWidgetItem* player2 = new QListWidgetItem("Nobody", connectedPlayersList);
	player2->setCheckState(Qt::Unchecked);
	// player2->setFlags(Qt::ItemIsUserCheckable); // Disabled WTF??
	player2->setFlags(Qt::ItemIsEnabled); // this disables checkable...
	// qDebug() << player2->flags();

	gameTypeEdit = new QComboBox;
	gameTypeEdit->addItem(tr("Coridor"));
	gameTypeEdit->addItem(tr("Quarto"));
	gameTypeEdit->setCurrentIndex(_gameType);

	connect(gameTypeEdit, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLobby(int)));

	startGameBtn = new QPushButton("Start");
	exitLobbyBtn = new QPushButton("Exit");

	lobbyLayout->addWidget(connectedPlayersList);
	lobbyLayout->addWidget(gameTypeEdit);
	lobbyLayout->addWidget(startGameBtn);
	lobbyLayout->addWidget(exitLobbyBtn);

	connect(exitLobbyBtn, SIGNAL(clicked()), this, SLOT(leaveLobby()));
	// connect()
}

void MainWindow::switchToLobbiesList()
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	lobbiesListLayout = new QGridLayout(centralWidget);

	lobbiesList = new QTreeWidget();
	lobbiesList->setColumnCount(4);
	QStringList headers;
	headers << tr("Lobby Name") << tr("Host") << tr("Game Type") << tr("Players Number");
	lobbiesList->setHeaderLabels(headers);
	updateLobbiesListBtn = new QPushButton("Update");
	exitLobbiesBtn = new QPushButton("Exit");

	lobbiesListLayout->addWidget(lobbiesList);
	lobbiesListLayout->addWidget(updateLobbiesListBtn);
	lobbiesListLayout->addWidget(exitLobbiesBtn);

	connect(updateLobbiesListBtn, SIGNAL(clicked()), this, SLOT(askLobbies()));
	connect(exitLobbiesBtn, SIGNAL(clicked()), this, SLOT(leaveLobbiesList()));
}

void MainWindow::sockDisc()
{
	pSocket->deleteLater();
	switchToLoginIn();
}

void MainWindow::createLobby(QString lobbyName, QString hostLogin, int gameType)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::CreateLobby};
	Command* pCommand = new CreateLobby(lobbyName, hostLogin, gameType);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "CreateLobby Command Sent";

	lobby = new Lobby(lobbyName, hostLogin, gameType);

	switchToLobby(gameType);
}

void MainWindow::deleteLobby(QString lobbyName)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::DeleteLobby};
	Command* pCommand = new DeleteLobby(lobbyName);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "DeleteLobby Command Sent";
	lobby = new Lobby();
}

void MainWindow::askLobbies()
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::AskLobbies};
	Command* pCommand = new AskLobbies();

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "AskLobbies Command Sent";
}

void MainWindow::updateLobby(int _gameType)
{
	lobby->update(_gameType);
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::ChangeGameType};
	Command* pCommand = new ChangeGameType(_gameType);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "ChangeGameType Command Sent";
}

void MainWindow::sockReady()
{
	CommandFactory factory;
	QDataStream in(pSocket);
	in.setVersion(QDataStream::Qt_5_9);
	pCommand = factory.create(in);
	pCommand->execute();
	switchCmd();
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

void MainWindow::switchCmd()
{
	if (SendLobbies* pSendLobbies = dynamic_cast<SendLobbies*>(pCommand))
	{
		switchToLobbiesList();

		list<QTreeWidgetItem*> lobbyItems;
		for (const auto& i : pSendLobbies->lobbies)
		{
			lobbyItems.push_back(new QTreeWidgetItem(lobbiesList));
			lobbyItems.back()->setText(0, i->lobbyName);
			lobbyItems.back()->setText(1, i->host->playerName);
			lobbyItems.back()->setText(2, i->gameTypeStr);
			/*switch (i->gameType)
			{
			case Coridor:
				lobbyItems.back()->setText(2, "Coridor");
				break;
			case Quarto:
				lobbyItems.back()->setText(2, "Quarto");
				break;
			}*/
			lobbyItems.back()->setText(3, QString::number(i->connectedPlayersNumber) + "/" + QString::number(i->maxPlayers));
			connect(lobbiesList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(connectToLobby(QTreeWidgetItem*, int)));
		}
	}
}
