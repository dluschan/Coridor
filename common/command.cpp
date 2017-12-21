#include "command.h"
#include <QDebug>

QDataStream& operator>>(QDataStream& stream, CommandType& commandType)
{
	stream >> commandType.type;
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const CommandType& commandType)
{
	stream << commandType.type;
	return stream;
}

Command* CommandFactory::create(QDataStream& stream) throw(std::logic_error)
{
	CommandType commandType;
	stream >> commandType;
	Command* command;
	switch (commandType.type)
	{
	case 0: // CommandType::AskLogin
		command = new Login();
		break;

	case 1: // CommandType::AskHelp
		command = new Help();
		break;

	default:
		throw std::logic_error("Incorrect type of command");
	}

	command->operator>>(stream);
	return command;
}

Login::Login(QString _login)
	: login(_login)
{
	qDebug() << "Login command created" << login;
}

void Login::execute()
{
	qDebug() << "Login is" << login;
}

QDataStream& Login::operator>>(QDataStream& stream)
{
	std::string s;
	QString q;
	stream >> q;
	login = QString::fromStdString(s);
	return stream;
}

QDataStream& Login::operator<<(QDataStream& stream) const
{
	std::string s = login.toStdString();
	QString q;
	stream << q;
	return stream;
}

Help::Help()
{
	qDebug() << "Help command created";
}

void Help::execute()
{
}

QDataStream& Help::operator>>(QDataStream& stream)
{
	return stream;
}

QDataStream& Help::operator<<(QDataStream& stream) const
{
	return stream;
}
