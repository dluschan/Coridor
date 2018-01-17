#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "../common/command.h"
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QtNetwork>
#include <exception>
#include <iostream>
#include <string>

class MyThread : public QThread
{
	Q_OBJECT
public:
	explicit MyThread(int ID, QObject* parent = 0);
	QTcpSocket* pSocket;
	Player* pPlayer;
	Lobby* pLobby = new Lobby();
	void run();
	void sendCreatePlayer(Player* _player);
	void sendMessage(QString message, bool error);
	Lobby* createLobby(QString lobbyName, int gameType);
	void sendCreateLobby(Lobby* _lobby);
	void sendUpdateLobby(int _gameType, int _status);
	void sendConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag);
	void sendDeleteLobby(Lobby* lobby);
	void write(QByteArray buffer);
	void sendRdy();
	void sendStart();
	void sendFirstPlayer(QString _firstPlayer, QString _guest, GameType _gameType);
	void sendQuit(QString _reciever);
	void coridorSendQPoint(QPoint point, bool move, QString enemy, bool horizontal);
	void quartoSendQPoint(QPoint point, int figureId, QString enemy);
	void quartoSendCheckWin(QString enemy, bool checkWin);

signals:
	void createPlayerSignal(Player* player, MyThread* thread);
	void deletePlayerSignal(MyThread* thread);
	void errorSignal(QTcpSocket::SocketError socketError);
	void sendPlayerListSignal(MyThread* thread);
	void createLobbySignal(Lobby* pLobby);
	// void changeGameTypeSignal(MyThread* thread, int gameType);
	void changeGameTypeSignal(MyThread* thread, int gameType, int _status);
	void sendGameTypesSignal(Player* player, int gameType, int _status);
	void deleteLobbyFromListSignal(Lobby* pLobby);
	void sendDeleteLobbySignal(Player* pPlayer);
	void connectToLobbySignal(Lobby* pLobby, Player* player, bool connectFlag);
	void sendLobbiesListSignal(MyThread* thread);
	void sendRdySignal(Player* host);
	void sendMessageSignal(QString message, bool errorFlag, QString playerName);
	void sendFirstPlayerSignal(QString firstPlayer, QString guest, GameType gameType);
	void sendQuitSignal(QString reciever);
	void coridorSendQPointSignal(QPoint point, bool move, QString enemy, bool horizontal);
	void quartoSendQPointSignal(QPoint point, int figureId, QString enemy);
	void quartoSendCheckWinSignal(QString enemy, bool checkWin);

public slots:
	void readyRead();
	void disconnected();
	void changeGameType(int _gameType);

private:
	int socketDescriptor;
	Command* pCommand;

	void login(QString login);
	void switchCmd();
};

#endif // MYTHREAD_H
