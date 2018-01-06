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

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();

	QTcpSocket* pSocket;
	QByteArray data;

public slots:
	void sockReady();
	void sockDisc();
	void createLobby(QString _lobbyName, QString _hostLogin, int _gameType);
	void askLobbies();

private slots:
	void connectToTheServer();
	void createLobbyDialog();
	void leaveLobby();

signals:

private:
	Player* player;
	Lobby* lobby;
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
	/*QLabel* hostName;
	QCheckBox* hostRdy;
	QLabel* connectedPlayerName;
	QCheckBox* connectedPlayerRdy;*/

	QComboBox* gameTypeEdit;
	QPushButton* startGameBtn;
	QPushButton* exitLobbyBtn;

	bool playerConnected = false;

	QGridLayout* lobbiesListLayout;
	QTreeWidget* lobbiesList;
	QPushButton* connectToLobby;
	QPushButton* exitLobbiesBtn;

	void switchToLoginIn();
	void switchToMain();
	void switchToLobby(int _gameType);
	void switchToLobbiesList();

	void switchCmd();
};

#endif // MAINWINDOW_H
