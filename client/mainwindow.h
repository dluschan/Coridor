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

private slots:
	void sockReady();
	void sockDisc();
	void createLobby(QString lobbyName, QString hostLogin, int gameType);
	void askLobbies();
	void updateLobby(int _gameType);

	void connectToTheServer();
	void createLobbyDialog();
	void sendConnectToLobbySlot(QTreeWidgetItem* item, int column);
	void leaveLobby();
	void deleteLobbySlot();
	void leaveLobbiesList();
	void setRdy();
	void sendRdy();
	void switchToCoridorWindow(bool hosting);
	void switchToQuartoWindow(bool host);
	void switchToGameLikeHostSlot();
	void returnFromGame();
	void choseFirstPlayer(QString _firstPlayer);
	void sendFirstPlayerSlot(QString _firstPlayer);
	void sendQPoint(QPoint point, bool move, QString enemy, bool horizontal);

signals:
	// void choseFirstPlayerSignal();
	void coridorSendQPointSignal(QPoint point, bool move, QString enemy, bool horizontal);

private:
	Player* pPlayer;
	Lobby* pLobby;
	Command* pCommand;
	CoridorWindow* coridorWindow;
	QuartoWindow* quartoWindow;

	QWidget* centralWidget;

	QGridLayout* connectLayout;
	QPushButton* connectBtn;
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

	DialogChosePlayer* dialogChoosePlayer;

	void switchToLoginIn();
	void switchToMain();
	void switchToLobby(Player* connectingPlayer = new Player(), Lobby* _lobby = new Lobby(), bool flagHosting = true);
	// void switchToLobbyGuest(Lobby* _lobby);
	void switchToLobbiesList();

	void deleteLobby(Lobby* lobby);
	void sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag);
	void sendUpdateLobby(int _gameType, int _status);
	void sendMessage(QString message);
	void sendChooseFirstPlayer(QString _firstPlayer, QString _guest);
	void switchToGame(bool hosting);
	void switchCmd();
};

#endif // MAINWINDOW_H
