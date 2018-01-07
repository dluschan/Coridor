#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
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
	void updateLobby(int);

	void connectToTheServer();
	void createLobbyDialog();
	void sendConnectToLobby(QTreeWidgetItem* item, int column);
	void leaveLobby();
	void deleteLobbySlot();
	void leaveLobbiesList();
	void setRdy();
	void sendRdy();

signals:

private:
	Player* pPlayer;
	Lobby* pLobby;
	Command* pCommand;

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

	void switchToLoginIn();
	void switchToMain();
	void switchToLobby(Player* connectingPlayer = new Player(), Lobby* _lobby = new Lobby(), int _gameType = 0, bool flagHosting = true);
	void switchToLobbyGuest(Lobby* _lobby);
	void switchToLobbiesList();

	void deleteLobby(Lobby* lobby);
	void sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag);
	void switchCmd();
};

#endif // MAINWINDOW_H
