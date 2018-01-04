#ifndef COMMAND_H
#define COMMAND_H

#include "lobby.h"
#include <QDataStream>
#include <QString>

using namespace std;

struct CommandType
{
	enum class Type : unsigned int
	{
		AskLogin,
		CreateLobby,
		AskLobbies,
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
	CreateLobby(QString _lobbyName = QString("Lobby Name"), QString _hostLogin = QString(), int _gameType = 1);

	Lobby* lobby;

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;

private:
	QString lobbyName;
	QString hostLogin;
	int gameType;
};

#endif // COMMAND_H
