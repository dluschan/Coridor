#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "coridorwindow.h"
#include "dialog.h"
#include "dialogchoseplayer.h"
#include "quartowindow.h"
#include <QCheckBox>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTcpSocket>
#include <QTime>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();

	QTcpSocket* pSocket;
	QByteArray data;

	/*protected:
		void closeEvent(QCloseEvent* event);*/

private slots:
	void sockReady();
	void sockDisc();
	void sendCreateLobby(QString lobbyName, QString hostLogin, int gameType);
	void askLobbies();
	void updateLobby(int _gameType);

	void connectToTheServer();
	void sendLogin();
	void createLobbyDialog();
	void sendConnectToLobbySlot(QTreeWidgetItem* item, int column);
	void leaveLobby();
	void deleteLobbySlot();
	void leaveLobbiesList();
	void setRdy();
	void sendRdy();
	void sendMessage(QString message, bool errorFlag, QString playerName);
	void switchToCoridorWindow(bool hosting);
	void switchToQuartoWindow(bool hosting);
	void switchToGameLikeHostSlot();
	void returnFromGame();
	// void sendQuit(QString _reciever);
	// void recieveQuit();
	void chooseFirstPlayerCoridor(QString _firstPlayer);
	void chooseFirstPlayerQuarto(QString _firstPlayer);
	void chooseFirstPlayer(QString _firstPlayer, GameType _gameType);
	void sendFirstPlayerCoridorSlot(QString _firstPlayer);
	void sendFirstPlayerQuartoSlot(QString _firstPlayer);
	void coridorSendQPoint(QPoint point, bool move, QString enemy, bool horizontal);
	void quartoSendQPoint(QPoint point, int figureId, QString enemy);
	void quartoSendCheckWin(QString enemy, bool checkWin);

signals:
	// void choseFirstPlayerSignal();
	// void sendQuitSignal();
	void coridorRecieveQPointSignal(QPoint point, bool move, QString enemy, bool horizontal);
	void quartoRecieveQPointSignal(QPoint point, int figureId, QString enemy);
	void quartoRecieveCheckWinSignal(QString enemy, bool checkWin);
	void gameEndSignal();
	void closeGame();

private:
	Player* pPlayer = new Player();
	Lobby* pLobby = new Lobby();
	Command* pCommand;
	CoridorWindow* coridorWindow;
	QuartoWindow* quartoWindow;

	QWidget* centralWidget;

	QGridLayout* connectLayout;
	QPushButton* connectBtn;
	QPushButton* loginBtn;
	QLineEdit* loginEdit;
	QLineEdit* passwordEdit;

	QGridLayout* mainLayout;
	QPushButton* createLobbyBtn;
	QPushButton* showLobbiesBtn;
	QPushButton* disconnectBtn;

	QGridLayout* lobbyLayout;
	QListWidget* connectedPlayersList;
	QComboBox* gameTypeEdit;
	QPushButton* startGameBtn;
	QPushButton* exitLobbyBtn;

	// bool playerConnected = false;

	QGridLayout* lobbiesListLayout;
	QTreeWidget* lobbiesList;
	QPushButton* updateLobbiesListBtn;
	QPushButton* exitLobbiesBtn;

	DialogChoosePlayer* dialogChoosePlayer;

	void switchToLoginIn();
	void switchToMain();
	void switchToLobby(Player* connectingPlayer = new Player(), Lobby* _lobby = new Lobby(), bool flagHosting = true);
	// void switchToLobbyGuest(Lobby* _lobby);
	void switchToLobbiesList();

	void deleteLobby(Lobby* lobby);
	void sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag);
	void sendUpdateLobby(int _gameType, int _status);
	void sendChooseFirstPlayer(QString _firstPlayer, QString _guest, GameType _gameType);
	void switchToGame(bool hosting);
	void switchCmd();
};

#endif // MAINWINDOW_H
