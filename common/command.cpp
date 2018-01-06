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
	case CommandType::Type::AskLogin:
		pCommand = new Login();
		break;

	case CommandType::Type::CreateLobby:
		pCommand = new CreateLobby();
		break;
	case CommandType::Type::DeleteLobby:
		pCommand = new DeleteLobby();
		break;
	case CommandType::Type::AskLobbies:
		pCommand = new AskLobbies();
		break;

	case CommandType::Type::SendLobbies:
	{
		list<Lobby*> lobbiesEmpty;
		pCommand = new SendLobbies(lobbiesEmpty);
		break;
	}
	case CommandType::Type::SendString:
		pCommand = new SendString();
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

void Login::execute()
{
	qDebug() << "Login is" << login;
	player = new Player(login);
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
	qDebug() << "execute CreateLobby command" << lobbyName << hostLogin << gameType;
}

DeleteLobby::DeleteLobby(QString _lobbyName)
//: lobbyName(_lobbyName)
{
	qDebug() << "DeleteLobby command created" << _lobbyName;
}

QDataStream& DeleteLobby::operator>>(QDataStream& stream)
{
	// qDebug() << "DeleteLobby read";
	// stream >> lobbyName;
	return stream;
}

QDataStream& DeleteLobby::operator<<(QDataStream& stream) const
{
	// qDebug() << "DeleteLobby written";
	// stream << lobbyName;
	return stream;
}

void DeleteLobby::execute()
{
	qDebug() << "execute DeleteLobby command";
}

AskLobbies::AskLobbies()
{
	qDebug() << "AskLobbies command created";
}

QDataStream& AskLobbies::operator>>(QDataStream& stream)
{
	return stream;
}

QDataStream& AskLobbies::operator<<(QDataStream& stream) const
{
	return stream;
}

void AskLobbies::execute()
{
	qDebug() << "execute AskLobbies command";
}

SendLobbies::SendLobbies(list<Lobby*> _lobbies)
	: lobbies(_lobbies)
{
	if (!lobbies.empty())
	{
		qDebug() << "SendLobbies command created";
	}
}

QDataStream& operator>>(QDataStream& stream, Lobby& lobby)
{
	QString lobbyName;
	QString hostHame;
	unsigned int gameType;

	stream >> lobbyName >> hostHame >> gameType;
	lobby = Lobby(lobbyName, hostHame, gameType);
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Lobby& lobby)
{
	stream << lobby.lobbyName << lobby.host->playerName << unsigned(lobby.gameType);
	return stream;
}

QDataStream& SendLobbies::operator>>(QDataStream& stream)
{
	qDebug() << "SendLobbies read";
	Lobby* lobbyTmp = new Lobby();
	int size;
	stream >> size;
	for (int i = 0; i < size; i++)
	{
		stream >> *lobbyTmp;
		lobbies.push_back(lobbyTmp);
		qDebug() << ">>>>>>>>>>>>>>>>>>";
	}
	return stream;
}

QDataStream& SendLobbies::operator<<(QDataStream& stream) const
{
	qDebug() << "SendLobbies written";
	stream << lobbies.size();
	for (const auto& i : lobbies)
	{
		stream << *i;
		qDebug() << "<<<<<<<<<<<<<<<<<<";
	}
	return stream;
}

void SendLobbies::execute()
{
	qDebug() << "execute SendLobbies command";
}

SendString::SendString(QString _message)
	: message(_message)
{
	qDebug() << "SendString command created";
}

QDataStream& SendString::operator>>(QDataStream& stream)
{
	qDebug() << "SendString read";
	stream >> message;
	return stream;
}

QDataStream& SendString::operator<<(QDataStream& stream) const
{
	qDebug() << "SendString written";
	stream << message;
	return stream;
}

void SendString::execute()
{
	qDebug() << message;
}
