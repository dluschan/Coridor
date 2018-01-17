#ifndef COMMAND_H
#define COMMAND_H

#include "lobby.h"
#include <QDataStream>
#include <QPoint>
#include <QString>
#include <list>

using namespace std;

struct CommandType
{
	enum class Type : unsigned int
	{
		AskLogin,
		CreateLobby,
		UpdateLobby,
		DeleteLobby,
		AskLobbies,
		SendLobbies,
		ConnectToLobby,
		SendRdy,
		SendFirstPlayer,
		// SendQuit,
		CoridorSendQPoint,
		QuartoSendQPoint,
		QuartoSendCheckWin,
		SendMessage,
		WrongCommand
	};
	Type type;
};

QDataStream& operator>>(QDataStream& stream, CommandType& type);
QDataStream& operator<<(QDataStream& stream, const CommandType& type);

class Command;

class CommandFactory
{
public:
	Command* create(QDataStream& stream) throw(std::logic_error);
};

class Command
{
public:
	virtual void execute() = 0;
	virtual QDataStream& operator>>(QDataStream& stream) = 0;
	virtual QDataStream& operator<<(QDataStream& stream) const = 0;
};

class Login : public Command
{
public:
	Login(QString _login = QString());

	Player* player;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;

private:
	QString login;
};

class CreateLobby : public Command
{
public:
	CreateLobby(Lobby* _lobby = new Lobby());

	Lobby* lobby;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;

private:
	QString lobbyName;
	QString hostLogin;
	int gameType;
};

class UpdateLobby : public Command
{
public:
	UpdateLobby(int _gameType, int _status, std::list<Player*> _connectedPlayers);

	int gameType, status;
	std::list<Player*> connectedPlayers;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class DeleteLobby : public Command
{
public:
	DeleteLobby(Lobby* _lobby = new Lobby());

	Lobby* lobby;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class AskLobbies : public Command
{
public:
	AskLobbies();

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class SendLobbies : public Command
{
public:
	SendLobbies(list<Lobby*> _lobbies);

	std::list<Lobby*> lobbies;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class ConnectToLobby : public Command
{
public:
	ConnectToLobby(Lobby* _lobby = new Lobby(), Player* _player = new Player(), bool _connectFlag = true);

	Lobby* lobby;
	Player* player;
	bool connectFlag;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class SendRdy : public Command
{
public:
	SendRdy(Player* _host = new Player());

	Player* host;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class SendFirstPlayer : public Command
{
public:
	SendFirstPlayer(QString _firstPlayer = QString(), QString _guest = QString(), int _gameType = WrongGameType);

	QString firstPlayer;
	QString guest;
	int gameType;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

/*class SendQuit : public Command
{
public:
	SendQuit(QString _reciever = QString());

	QString reciever;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};*/

class CoridorSendQPoint : public Command
{
public:
	CoridorSendQPoint(QPoint _point = QPoint(), bool _move = true, QString _enemy = QString(), bool _horizontal = true);

	QPoint point;
	bool move;
	QString enemy;
	bool horizontal;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class QuartoSendQPoint : public Command
{
public:
	QuartoSendQPoint(QPoint _point = QPoint(), int _figureId = -1, QString _enemy = QString());

	QPoint point;
	int figureId;
	QString enemy;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class QuartoSendCheckWin : public Command
{
public:
	QuartoSendCheckWin(QString _enemy = QString(), bool _checkWin = false);

	QString enemy;
	bool checkWin; // false -> nextTurn

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class SendMessage : public Command
{
public:
	SendMessage(QString _message = QString(), bool _error = false, QString _playerName = QString());

	QString message;
	bool errorFlag;
	QString playerName;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

#endif // COMMAND_H
