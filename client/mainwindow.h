#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTcpSocket>
#include <QTime>

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

private slots:
	void connectToTheServer();
	void createLobbyDialog();

signals:

private:
	QWidget* centralWidget;
	QWidget* centralWidget2;
	QVBoxLayout* connectLayout;
	QPushButton* connectBtn;
	QLineEdit* loginEdit;
	QLineEdit* passwordEdit;
	QVBoxLayout* mainLayout;
	QPushButton* createLobbyBtn;
	QPushButton* showLobbiesBtn;
	QPushButton* disconnectBtn;

	void switchToLoginIn();
	void switchToMain();
};

#endif // MAINWINDOW_H
