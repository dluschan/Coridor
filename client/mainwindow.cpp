#include "mainwindow.h"
#include <QRegExp>

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
	Command* pCommand = new Login(loginEdit->text());

	pPlayer = new Player(loginEdit->text());
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
	CreateLobbyDialog* dialog = new CreateLobbyDialog(pPlayer->playerName);
	connect(dialog, SIGNAL(clicked(QString, QString, int)), this, SLOT(createLobby(QString, QString, int)));
	// (dialog->lobbyNameEdit->text(), dialog->hostLoginEdit->text(), dialog->gameTypeEdit->currentIndex())
}

void MainWindow::sendConnectToLobby(QTreeWidgetItem* item, int column)
{
	/*QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::ConnectToLobby};*/

	// получение connectedPlayersNumber
	QRegExp rx(".*\\s*(\\d+)\\s*/.*");
	int pos = 0;

	int connectedPlayersNumber;
	if ((pos = rx.indexIn(item->text(3), 0)) != -1)
	{
		connectedPlayersNumber = rx.cap(1).toInt();
	}

	pLobby = new Lobby(item->text(0), item->text(1), pLobby->getGameType(item->text(2)), connectedPlayersNumber);
	sendConnectToLobby(pLobby, pPlayer, true);
	/*Command* pCommand = new ConnectToLobby(pLobby, pPlayer);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "ConnectToLobby Command Sent";

	pLobby->connect(pPlayer);*/
}

void MainWindow::leaveLobby()
{
	sendConnectToLobby(pLobby, pPlayer, false);
	pLobby = new Lobby();
	switchToMain();
}

void MainWindow::deleteLobby()
{
	deleteLobby(pLobby->lobbyName);
	pLobby = new Lobby();
	switchToMain();
}

void MainWindow::leaveLobbiesList()
{
	switchToMain();
}

void MainWindow::setRdy()
{
	// qDebug() << connectedPlayersList->item(1)->text();
	if (connectedPlayersList->item(1)->checkState())
		connectedPlayersList->item(1)->setCheckState(Qt::Unchecked);
	else
		connectedPlayersList->item(1)->setCheckState(Qt::Checked);
}

void MainWindow::sendRdy()
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendRdy};
	Command* pCommand = new SendRdy(pLobby->host);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "SendRdy Command Sent";
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

void MainWindow::switchToLobby(Player* connectingPlayer, Lobby* _lobby, int _gameType, bool flagHosting)
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	lobbyLayout = new QGridLayout(centralWidget);

	connectedPlayersList = new QListWidget();

	QListWidgetItem* player1 = new QListWidgetItem("Player1", connectedPlayersList); // pPlayer->playerName
	player1->setCheckState(Qt::Checked);
	player1->setFlags(Qt::ItemIsEnabled); // this disables checkable...

	QListWidgetItem* player2 = new QListWidgetItem("Player2", connectedPlayersList);
	player2->setCheckState(Qt::Unchecked);
	player2->setFlags(Qt::ItemIsEnabled); // this disables checkable...

	gameTypeEdit = new QComboBox;
	gameTypeEdit->addItem(tr("Coridor"));
	gameTypeEdit->addItem(tr("Quarto"));
	gameTypeEdit->setCurrentIndex(_gameType);
	connect(gameTypeEdit, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLobby(int)));

	exitLobbyBtn = new QPushButton("Exit");

	if (flagHosting)
	{
		player1->setText(pPlayer->playerName);
		player2->setText("");

		gameTypeEdit->setEnabled(true);

		startGameBtn = new QPushButton("Start");
		connect(exitLobbyBtn, SIGNAL(clicked()), this, SLOT(deleteLobby()));
	}
	else
	{
		if (connectingPlayer->playerName != pPlayer->playerName)
		{
			player1->setText(pPlayer->playerName);
			player2->setText(connectingPlayer->playerName);

			gameTypeEdit->setEnabled(true);

			startGameBtn = new QPushButton("Start");
			connect(exitLobbyBtn, SIGNAL(clicked()), this, SLOT(deleteLobby()));
			pLobby->connect(connectingPlayer);
		}
		else
		{
			pLobby = _lobby;
			player1->setText(pLobby->host->playerName);
			player2->setText(pPlayer->playerName);

			gameTypeEdit->setEnabled(false);

			startGameBtn = new QPushButton("Ready");
			connect(exitLobbyBtn, SIGNAL(clicked()), this, SLOT(leaveLobby()));
			connect(startGameBtn, SIGNAL(clicked()), this, SLOT(setRdy())); // maybe will need player index
			connect(startGameBtn, SIGNAL(clicked()), this, SLOT(sendRdy()));
		}
	}

	lobbyLayout->addWidget(connectedPlayersList);
	lobbyLayout->addWidget(gameTypeEdit);
	lobbyLayout->addWidget(startGameBtn);
	lobbyLayout->addWidget(exitLobbyBtn);

	// connect()
}

void MainWindow::switchToLobbyGuest(Lobby* _lobby)
{
	pLobby = _lobby;

	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	lobbyLayout = new QGridLayout(centralWidget);

	connectedPlayersList = new QListWidget();

	QListWidgetItem* player1 = new QListWidgetItem(pLobby->host->playerName, connectedPlayersList);
	player1->setCheckState(Qt::Checked);
	player1->setFlags(Qt::ItemIsEnabled); // this disables checkable...

	QListWidgetItem* player2 = new QListWidgetItem(pPlayer->playerName, connectedPlayersList);
	player2->setCheckState(Qt::Unchecked);
	player2->setFlags(Qt::ItemIsEnabled); // this disables checkable...

	gameTypeEdit = new QComboBox;
	gameTypeEdit->addItem(tr("Coridor"));
	gameTypeEdit->addItem(tr("Quarto"));
	gameTypeEdit->setCurrentIndex(pLobby->gameType);

	// connect(gameTypeEdit, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLobby(int)));

	startGameBtn = new QPushButton("Ready");
	exitLobbyBtn = new QPushButton("Exit");

	lobbyLayout->addWidget(connectedPlayersList);
	lobbyLayout->addWidget(gameTypeEdit);
	lobbyLayout->addWidget(startGameBtn);
	lobbyLayout->addWidget(exitLobbyBtn);

	connect(exitLobbyBtn, SIGNAL(clicked()), this, SLOT(leaveLobby()));
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
	Command* pCommand = new CreateLobby(new Lobby(lobbyName, hostLogin, gameType));

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "CreateLobby Command Sent";

	pLobby = new Lobby(lobbyName, hostLogin, gameType);

	switchToLobby(new Player(), new Lobby(), gameType, true);
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
	pLobby = new Lobby();
}

void MainWindow::sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::ConnectToLobby};

	Command* pCommand = new ConnectToLobby(_lobby, _player, _connectFlag);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "ConnectToLobby Command Sent" << _connectFlag;

	if (_connectFlag)
		pLobby->connect(_player);
	else
		pLobby->disconnect(_player);
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
	pLobby->updateGameType(_gameType);
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::ChangeGameType};
	if (!pLobby->connectedPlayers.empty())
		qDebug() << pLobby->connectedPlayers.back()->playerName;
	Command* pCommand = new ChangeGameType(_gameType, pLobby->connectedPlayers);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "ChangeGameType Command Sent";

	pLobby->updateGameType(_gameType);

	/*QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::CreateLobby};
	Command* pCommand = new CreateLobby(lobby->lobbyName, pPlayer->playerName, _gameType);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "CreateLobby Command Sent";*/
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
			connect(lobbiesList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(sendConnectToLobby(QTreeWidgetItem*, int)));
		}
	}
	else if (ChangeGameType* pChangeGameType = dynamic_cast<ChangeGameType*>(pCommand))
	{
		pLobby->updateGameType(pChangeGameType->gameType);
		switchToLobby(pPlayer, pLobby, pChangeGameType->gameType, false);
	}
	else if (ConnectToLobby* pConnectToLobby = dynamic_cast<ConnectToLobby*>(pCommand))
	{
		if (pConnectToLobby->connectFlag)
			switchToLobby(pConnectToLobby->player, pConnectToLobby->lobby, pConnectToLobby->lobby->getGameType(pConnectToLobby->lobby->gameTypeStr), false);
		else
		{
			switchToLobby(pConnectToLobby->player, pConnectToLobby->lobby, pConnectToLobby->lobby->getGameType(pConnectToLobby->lobby->gameTypeStr), true);
			pLobby->disconnect(pConnectToLobby->player);
		}
	}
	else if (SendRdy* pSendRdy = dynamic_cast<SendRdy*>(pCommand))
	{
		setRdy();
	}
	else if (SendMessage* pSendMessage = dynamic_cast<SendMessage*>(pCommand))
	{
		if (pSendMessage->errorFlag)
			QMessageBox::information(this, tr("Error"), pSendMessage->message);
	}
}
