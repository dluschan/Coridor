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
	pSocket->connectToHost("25.83.194.201", 5555);

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

void MainWindow::sendConnectToLobbySlot(QTreeWidgetItem* item, int column)
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
	pLobby->host->playerName;
	// pLobby->connectedPlayers.push_back(pPlayer); //Лобби подключаемого игрока хранит в connectedPlayers только своего Player*!!!
	// qDebug() << pLobby->connectedPlayers.back()->playerName;
	sendConnectToLobby(pLobby, pPlayer, true); //Лобби подключаемого игрока хранит в connectedPlayers только своего Player*!!!
	if (pLobby->connectedPlayers.empty())
		qDebug() << "sendConnectToLobbySlot ERROR - WTFFFFFFFFFFFF";
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

void MainWindow::deleteLobbySlot()
{
	deleteLobby(pLobby);
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
	{
		connectedPlayersList->item(1)->setCheckState(Qt::Unchecked);
		pLobby->updateStatus(Unready);
	}
	else
	{
		connectedPlayersList->item(1)->setCheckState(Qt::Checked);
		pLobby->updateStatus(Ready);
	}
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

void MainWindow::switchToCoridorWindow(bool hosting)
{
	this->close(); // Закрываем основное окно
	if (hosting)
	{
		if (!pLobby->connectedPlayers.empty())
		{
			dialogChoosePlayer = new DialogChoosePlayer(hosting, pLobby->host->playerName, pLobby->connectedPlayers.back()->playerName);
			connect(dialogChoosePlayer, SIGNAL(clicked(QString)), this, SLOT(chooseFirstPlayerCoridor(QString)));
			connect(dialogChoosePlayer, SIGNAL(clicked(QString)), this, SLOT(sendFirstPlayerCoridorSlot(QString)));
		}
		else
			qDebug() << "EROR THAT SHOULDN'T HAPPEN!!!!";
	}
	else
	{
		dialogChoosePlayer = new DialogChoosePlayer(hosting);
	}
}

void MainWindow::switchToQuartoWindow(bool hosting)
{
	this->close(); // Закрываем основное окно
	if (hosting)
	{
		if (!pLobby->connectedPlayers.empty())
		{
			dialogChoosePlayer = new DialogChoosePlayer(hosting, pLobby->host->playerName, pLobby->connectedPlayers.back()->playerName);
			connect(dialogChoosePlayer, SIGNAL(clicked(QString)), this, SLOT(chooseFirstPlayerQuarto(QString)));
			connect(dialogChoosePlayer, SIGNAL(clicked(QString)), this, SLOT(sendFirstPlayerQuartoSlot(QString)));
		}
		else
			qDebug() << "EROR THAT SHOULDN'T HAPPEN!!!!";
	}
	else
	{
		dialogChoosePlayer = new DialogChoosePlayer(hosting);
	}
	// chooseFirstPlayerQuarto
	// quartoWindow = new QuartoWindow();
	// quartoWindow->show(); // Показываем второе окно
	// this->close();		  // Закрываем основное окно
}

void MainWindow::switchToGame(bool hosting)
{
	if (pLobby->status == InGame)
	{
		switch (pLobby->gameType)
		{
		case Coridor:
			switchToCoridorWindow(hosting);
			break;
		case Quarto:
			switchToQuartoWindow(hosting);
			break;
		}
	}
}

void MainWindow::switchToGameLikeHostSlot()
{
	if (pLobby->status == Ready)
	{
		pLobby->updateStatus(InGame);
		sendUpdateLobby(pLobby->gameType, pLobby->status);
		switchToGame(true);
	}
}

void MainWindow::returnFromGame()
{
	pLobby = new Lobby();
	this->show();
	switchToMain();
}

void MainWindow::chooseFirstPlayerCoridor(QString _firstPlayer)
{
	QString _secondPlayer;
	if (_firstPlayer == pLobby->host->playerName)
		_secondPlayer = pLobby->connectedPlayers.back()->playerName; // CRASHES HERE CAUSE THERE IS NO connectedPlayers in pLobby
	else
		_secondPlayer = pLobby->host->playerName;

	// createGame(_firstPlayer, _secondPlayer, Coridor);
	coridorWindow = new CoridorWindow(_firstPlayer, _secondPlayer, pPlayer->playerName);
	connect(coridorWindow, SIGNAL(coridorSendQPointSignal(QPoint, bool, QString, bool)), this, SLOT(coridorSendQPoint(QPoint, bool, QString, bool)));
	connect(this, SIGNAL(coridorRecieveQPointSignal(QPoint, bool, QString, bool)), coridorWindow, SLOT(coridorRecieveQPoint(QPoint, bool, QString, bool)));
	connect(coridorWindow, SIGNAL(firstWindow()), this, SLOT(returnFromGame()));
}

void MainWindow::chooseFirstPlayerQuarto(QString _firstPlayer)
{
	QString _secondPlayer;
	if (_firstPlayer == pLobby->host->playerName)
		_secondPlayer = pLobby->connectedPlayers.back()->playerName; // CRASHES HERE CAUSE THERE IS NO connectedPlayers in pLobby
	else
		_secondPlayer = pLobby->host->playerName;
	// createGame(_firstPlayer, _secondPlayer, Quarto); sendQPointSignal
	quartoWindow = new QuartoWindow(_firstPlayer, _secondPlayer, pPlayer->playerName);
	connect(quartoWindow, SIGNAL(sendQPointSignal(QPoint, int, QString)), this, SLOT(quartoSendQPoint(QPoint, int, QString)));
	connect(this, SIGNAL(quartoRecieveQPointSignal(QPoint, int, QString)), quartoWindow, SLOT(quartoRecieveQPoint(QPoint, int, QString)));
	connect(quartoWindow, SIGNAL(quartoSendCheckWinSignal(QString, bool)), this, SLOT(quartoSendCheckWin(QString, bool)));
	connect(this, SIGNAL(quartoRecieveCheckWinSignal(QString, bool)), quartoWindow, SLOT(quartoRecieveCheckWin(QString, bool)));
	connect(quartoWindow, SIGNAL(firstWindow()), this, SLOT(returnFromGame()));
}

void MainWindow::chooseFirstPlayer(QString _firstPlayer, GameType _gameType)
{
	switch (_gameType)
	{
	case Coridor:
		chooseFirstPlayerCoridor(_firstPlayer);
		break;
	case Quarto:
		chooseFirstPlayerQuarto(_firstPlayer);
		break;
	default:
		qDebug() << "ChooseFirstPlayer Error";
		break;
	}
}

void MainWindow::sendFirstPlayerCoridorSlot(QString _firstPlayer)
{
	for (const auto& i : pLobby->connectedPlayers)
		sendChooseFirstPlayer(_firstPlayer, i->playerName, Coridor);
}

void MainWindow::sendFirstPlayerQuartoSlot(QString _firstPlayer)
{
	for (const auto& i : pLobby->connectedPlayers)
		sendChooseFirstPlayer(_firstPlayer, i->playerName, Quarto);
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

void MainWindow::switchToLobby(Player* connectingPlayer, Lobby* _lobby, bool flagHosting)
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	lobbyLayout = new QGridLayout(centralWidget);

	connectedPlayersList = new QListWidget();

	QListWidgetItem* player1 = new QListWidgetItem("Player1", connectedPlayersList); // pPlayer->playerName
	player1->setCheckState(Qt::Checked);
	player1->setFlags(Qt::ItemIsEnabled); // this disables checkable...

	QListWidgetItem* player2 = new QListWidgetItem("Player2", connectedPlayersList);
	if (pLobby->status == Unready)
		player2->setCheckState(Qt::Unchecked);
	else if (pLobby->status == Ready)
		player2->setCheckState(Qt::Checked);
	player2->setFlags(Qt::ItemIsEnabled); // this disables checkable...

	gameTypeEdit = new QComboBox;
	gameTypeEdit->addItem(tr("Coridor"));
	gameTypeEdit->addItem(tr("Quarto"));
	gameTypeEdit->setCurrentIndex(pLobby->gameType);
	connect(gameTypeEdit, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLobby(int)));

	exitLobbyBtn = new QPushButton("Exit");

	if (flagHosting)
	{
		player1->setText(pPlayer->playerName);
		player2->setText("");

		gameTypeEdit->setEnabled(true);

		startGameBtn = new QPushButton("Start");
		connect(startGameBtn, SIGNAL(clicked()), this, SLOT(switchToGameLikeHostSlot()));
		connect(exitLobbyBtn, SIGNAL(clicked()), this, SLOT(deleteLobbySlot()));
	}
	else
	{
		if (connectingPlayer->playerName != pPlayer->playerName)
		{
			player1->setText(pPlayer->playerName);
			player2->setText(connectingPlayer->playerName);

			gameTypeEdit->setEnabled(true);

			startGameBtn = new QPushButton("Start");
			connect(startGameBtn, SIGNAL(clicked()), this, SLOT(switchToGameLikeHostSlot()));
			connect(exitLobbyBtn, SIGNAL(clicked()), this, SLOT(deleteLobbySlot()));
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
			// connect(startGameBtn, SIGNAL(clicked()), this, SLOT(setRdy())); // maybe will need player index
			connect(startGameBtn, SIGNAL(clicked()), this, SLOT(sendRdy()));
		}
	}

	lobbyLayout->addWidget(connectedPlayersList);
	lobbyLayout->addWidget(gameTypeEdit);
	lobbyLayout->addWidget(startGameBtn);
	lobbyLayout->addWidget(exitLobbyBtn);

	// connect()
}

/*void MainWindow::switchToLobbyGuest(Lobby* _lobby)
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
}*/

void MainWindow::switchToLobbiesList()
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	lobbiesListLayout = new QGridLayout(centralWidget);

	lobbiesList = new QTreeWidget();
	lobbiesList->setColumnCount(5);
	QStringList headers;
	headers << tr("Lobby Name") << tr("Host") << tr("Game Type") << tr("Players Number") << tr("Status");
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

	switchToLobby(new Player(), new Lobby(), true);
}

void MainWindow::deleteLobby(Lobby* lobby)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::DeleteLobby};
	Command* pCommand = new DeleteLobby(lobby);

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

void MainWindow::sendUpdateLobby(int _gameType, int _status)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::UpdateLobby};
	Command* pCommand = new UpdateLobby(_gameType, _status, pLobby->connectedPlayers);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "ChangeGameType Command Sent";
}

void MainWindow::sendMessage(QString message)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendMessage};
	Command* pCommand = new SendMessage(message, false);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "SendMessage Command Sent";
}

void MainWindow::sendChooseFirstPlayer(QString _firstPlayer, QString _guest, GameType _gameType)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);

	CommandType commandType = {CommandType::Type::SendFirstPlayer};
	Command* pCommand = new SendFirstPlayer(_firstPlayer, _guest, _gameType);

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	qDebug() << "CreateLobby Command Sent";
}

void MainWindow::coridorSendQPoint(QPoint point, bool move, QString enemy, bool horizontal)
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

void MainWindow::quartoSendQPoint(QPoint point, int figureId, QString enemy)
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

	qDebug() << "QuartoSendQPoint Command Sent";
}

void MainWindow::quartoSendCheckWin(QString enemy, bool checkWin)
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
	sendUpdateLobby(_gameType, pLobby->status);
}

void MainWindow::sockReady()
{
	/*if (pLobby->connectedPlayers.empty())
		qDebug() << "11111111111111111111111111111";*/

	CommandFactory factory;
	QDataStream in(pSocket);
	in.setVersion(QDataStream::Qt_5_9);
	pCommand = factory.create(in);
	pCommand->execute();
	switchCmd();

	/*if (pLobby->connectedPlayers.empty())
		qDebug() << "222222222222222222222222222222222222222222222222";*/
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
			lobbyItems.back()->setText(3, QString::number(i->connectedPlayersNumber) + "/" + QString::number(i->maxPlayers));
			lobbyItems.back()->setText(4, i->statusStr);
			connect(lobbiesList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(sendConnectToLobbySlot(QTreeWidgetItem*, int)));
		}
	}
	else if (DeleteLobby* pDeleteLobby = dynamic_cast<DeleteLobby*>(pCommand))
	{
		switchToMain();
		pLobby = new Lobby();
	}
	else if (UpdateLobby* pUpdateLobby = dynamic_cast<UpdateLobby*>(pCommand))
	{
		pLobby->update(pUpdateLobby->gameType, pUpdateLobby->status);
		if (pLobby->connectedPlayers.empty())
			qDebug() << "UpdateLobby ERROR";
		if (pUpdateLobby->status != InGame)
		{
			for (const auto& i : pUpdateLobby->connectedPlayers)
				if (i->playerName == pPlayer->playerName)
					switchToLobby(i, pLobby, false);
		}
		else
			switchToGame(false);
		// switchToLobby(pPlayer, pLobby, false);
	}
	else if (ConnectToLobby* pConnectToLobby = dynamic_cast<ConnectToLobby*>(pCommand))
	{
		if (pConnectToLobby->connectFlag)
			//***** GameType
			switchToLobby(pConnectToLobby->player, pConnectToLobby->lobby, false);
		else
		{
			//***** GameType
			switchToLobby(pConnectToLobby->player, pConnectToLobby->lobby, true);
			pLobby->disconnect(pConnectToLobby->player);
		}
	}
	else if (SendRdy* pSendRdy = dynamic_cast<SendRdy*>(pCommand))
	{
		if (pLobby->connectedPlayers.empty())
			qDebug() << "SendRdy ERROR";
		setRdy();
	}
	else if (SendMessage* pSendMessage = dynamic_cast<SendMessage*>(pCommand))
	{
		if (pSendMessage->errorFlag)
			QMessageBox::information(this, tr("Error"), pSendMessage->message);
	}
	else if (SendFirstPlayer* pSendFirstPlayer = dynamic_cast<SendFirstPlayer*>(pCommand))
	{
		dialogChoosePlayer->hide();
		chooseFirstPlayer(pSendFirstPlayer->firstPlayer, (GameType)pSendFirstPlayer->gameType);
	}
	else if (CoridorSendQPoint* pCoridorSendQPoint = dynamic_cast<CoridorSendQPoint*>(pCommand))
	{
		emit coridorRecieveQPointSignal(pCoridorSendQPoint->point, pCoridorSendQPoint->move, pCoridorSendQPoint->enemy, pCoridorSendQPoint->horizontal);
	}
	else if (QuartoSendQPoint* pQuartoSendQPoint = dynamic_cast<QuartoSendQPoint*>(pCommand))
	{
		emit quartoRecieveQPointSignal(pQuartoSendQPoint->point, pQuartoSendQPoint->figureId, pQuartoSendQPoint->enemy);
	}
	else if (QuartoSendCheckWin* pQuartoSendCheckWin = dynamic_cast<QuartoSendCheckWin*>(pCommand))
	{
		emit quartoRecieveCheckWinSignal(pQuartoSendCheckWin->enemy, pQuartoSendCheckWin->checkWin);
	}
}
