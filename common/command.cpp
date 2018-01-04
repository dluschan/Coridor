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

	case CommandType::Type::CreateLobby: // CommandType::AskHelp
		pCommand = new CreateLobby();
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
	player = new Player(login);
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

CreateLobby::CreateLobby(QString _lobbyName, QString _hostLogin, int _gameType)
	: lobbyName(_lobbyName)
	, hostLogin(_hostLogin)
	, gameType(_gameType)
{
	qDebug() << "CreateLobby command created" << _lobbyName;
}

QDataStream& CreateLobby::operator>>(QDataStream& stream)
{
	qDebug() << "CreateLobby read";
	stream >> lobbyName >> hostLogin >> gameType;
	return stream;
}

QDataStream& CreateLobby::operator<<(QDataStream& stream) const
{
	qDebug() << "CreateLobby written";
	stream << lobbyName << hostLogin << gameType;
	return stream;
}

void CreateLobby::execute()
{
	lobby = new Lobby(lobbyName, hostLogin, gameType);
	qDebug() << "execute CreateLobby command";
}
