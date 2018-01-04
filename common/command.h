#ifndef COMMAND_H
#define COMMAND_H

#include <QDataStream>
#include <QString>

using namespace std;

struct CommandType
{
	enum class Type : unsigned int
	{
		AskLogin = 0,
		AskHelp = 1,
		AskPlayers = 2,
		CreateLobby = 3,
		AskLobbies = 4,
		WrongCommand = 5
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

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;

private:
	QString login;
};

class Help : public Command
{
public:
	Help();

	virtual void execute();
	virtual QDataStream& operator>>(QDataStream& stream);
	virtual QDataStream& operator<<(QDataStream& stream) const;
};

#endif // COMMAND_H
