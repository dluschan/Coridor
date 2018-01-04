#include "command.h"
#include <QDebug>

QDataStream& operator>>(QDataStream& stream, CommandType& commandType)
{
	unsigned int type;
	stream >> type;
	commandType.type = CommandType::Type(type);
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const CommandType& commandType)
{
	stream << unsigned(commandType.type);
	return stream;
}

Command* CommandFactory::create(QDataStream& stream) throw(std::logic_error)
{
	CommandType commandType;
	stream >> commandType;
	Command* pCommand;
	switch (commandType.type)
	{
	case CommandType::Type::AskLogin: // CommandType::AskLogin
		pCommand = new Login();
		break;

	case CommandType::Type::AskHelp: // CommandType::AskHelp
		pCommand = new Help();
		break;

	default:
		throw std::logic_error("Incorrect type of command");
	}

	pCommand->operator>>(stream);
	return pCommand;
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
	qDebug() << "login read";
	stream >> login;
	return stream;
}

QDataStream& Login::operator<<(QDataStream& stream) const
{
	qDebug() << "login written";
	stream << login;
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
