#ifndef COMMAND_H
#define COMMAND_H

#include "lobby.h"
#include <QDataStream>
#include <QString>
#include <list>

using namespace std;

struct CommandType
{
	enum class Type : unsigned int
	{
		AskLogin,
		CreateLobby,
		ChangeGameType,
		DeleteLobby,
		AskLobbies,
		SendLobbies,
		ConnectToLobby,
		// SendConnect,
		SendRdy,
		SendError,
		SendString,
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

/*class CreateLobby : public Command
{
public:
	CreateLobby(QString _lobbyName = QString("Lobby Name"), QString _hostLogin = QString(), int _gameType = 1);

	Lobby* lobby;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;

private:
	QString lobbyName;
	QString hostLogin;
	int gameType;
};*/

class ChangeGameType : public Command
{
public:
	ChangeGameType(int _gameType, /*Player* _host,*/ std::list<Player*> _connectedPlayers);

	int gameType;
	// Player* host;
	std::list<Player*> connectedPlayers;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class DeleteLobby : public Command
{
public:
	DeleteLobby(QString _lobbyName = QString("Lobby Name"));

	// QString lobbyName;

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
	// Lobby* lobbyTmp;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

class ConnectToLobby : public Command
{
public:
	ConnectToLobby(Lobby* _lobby = new Lobby(), Player* _player = new Player());

	Lobby* lobby;
	Player* player;

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

/*class SendConnect : public Command
{
public:
	SendConnect(Lobby* _lobby = new Lobby(), Player* _player = new Player());

	Lobby* lobby;
	Player* player;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};*/

class SendString : public Command
{
public:
	SendString(QString _message = QString());

	QString message;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

#endif // COMMAND_H
