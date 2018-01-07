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
	case CommandType::Type::ChangeGameType:
	{
		list<Player*> playerEmpty;
		pCommand = new ChangeGameType(0, playerEmpty);
		break;
	}
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
	case CommandType::Type::ConnectToLobby:
		pCommand = new ConnectToLobby();
		break;
	case CommandType::Type::SendRdy:
		pCommand = new SendRdy();
		break;
	case CommandType::Type::SendMessage:
		pCommand = new SendMessage();
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

CreateLobby::CreateLobby(Lobby* _lobby)
	: lobby(_lobby)
{
	qDebug() << "CreateLobby command created";
}

QDataStream& operator>>(QDataStream& stream, Lobby& lobby)
{
	QString lobbyName;
	QString hostHame;
	unsigned int gameType;
	int connectedPlayersNumber;

	stream >> lobbyName >> hostHame >> gameType >> connectedPlayersNumber;
	lobby = Lobby(lobbyName, hostHame, gameType, connectedPlayersNumber);
	// lobby.connectedPlayersNumber = connectedPlayersNumber;
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Lobby& lobby)
{
	stream << lobby.lobbyName << lobby.host->playerName << unsigned(lobby.gameType) << lobby.connectedPlayersNumber;
	return stream;
}

QDataStream& CreateLobby::operator>>(QDataStream& stream)
{
	qDebug() << "CreateLobby read";
	stream >> *lobby;
	return stream;
}

QDataStream& CreateLobby::operator<<(QDataStream& stream) const
{
	qDebug() << "CreateLobby written";
	stream << *lobby;
	return stream;
}

void CreateLobby::execute()
{
	qDebug() << "execute CreateLobby command";
}

/*CreateLobby::CreateLobby(QString _lobbyName, QString _hostLogin, int _gameType)
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
}*/

ChangeGameType::ChangeGameType(int _gameType, std::list<Player*> _connectedPlayers)
	: gameType(_gameType)
	//, host(_host)
	, connectedPlayers(_connectedPlayers)
{
	qDebug() << "ChangeGameType command created" << gameType;
	// connectedPlayers.push_back(host);
}

QDataStream& operator>>(QDataStream& stream, Player& player)
{
	QString login;

	stream >> login;
	player = Player(login);
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Player& player)
{
	stream << player.playerName;
	return stream;
}

QDataStream& ChangeGameType::operator>>(QDataStream& stream)
{
	qDebug() << "ChangeGameType read";

	Player* playerTmp = new Player();
	int size;
	stream >> size;
	stream >> gameType;
	for (int i = 0; i < size; i++)
	{
		stream >> *playerTmp;
		connectedPlayers.push_back(playerTmp);
	}

	return stream;
}

QDataStream& ChangeGameType::operator<<(QDataStream& stream) const
{
	qDebug() << "ChangeGameType written";

	stream << connectedPlayers.size();
	stream << gameType;

	for (const auto& i : connectedPlayers)
	{
		stream << *i;
	}

	return stream;
}

void ChangeGameType::execute()
{
	qDebug() << "execute ChangeGameType command" << gameType;
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
	}
	return stream;
}

void SendLobbies::execute()
{
	qDebug() << "execute SendLobbies command";
}

ConnectToLobby::ConnectToLobby(Lobby* _lobby, Player* _player, bool _connectFlag)
	: lobby(_lobby)
	, player(_player)
	, connectFlag(_connectFlag)
{
	qDebug() << "ConnectToLobby command created";
}

QDataStream& ConnectToLobby::operator>>(QDataStream& stream)
{
	qDebug() << "ConnectToLobby read";
	stream >> *lobby >> *player >> connectFlag;
	return stream;
}

QDataStream& ConnectToLobby::operator<<(QDataStream& stream) const
{
	qDebug() << "ConnectToLobby written";
	stream << *lobby << *player << connectFlag;
	return stream;
}

void ConnectToLobby::execute()
{
	qDebug() << "execute ConnectToLobby command";
}

SendRdy::SendRdy(Player* _host)
	: host(_host)
{
	qDebug() << "SendRdy command created" << host->playerName;
}

QDataStream& SendRdy::operator>>(QDataStream& stream)
{
	stream >> *host;
	return stream;
}

QDataStream& SendRdy::operator<<(QDataStream& stream) const
{
	stream << *host;
	return stream;
}

void SendRdy::execute()
{
	qDebug() << "execute SendRdy command" << host->playerName;
}

/* SendConnect::SendConnect(Lobby* _lobby, Player* _player)
	: lobby(_lobby)
, player(_player)
{
	qDebug() << "SendConnect command created";
}

QDataStream& SendConnect::operator>>(QDataStream& stream)
{
	qDebug() << "SendConnect read";
	stream >> *lobby >> *player;
	return stream;
}

QDataStream& SendConnect::operator<<(QDataStream& stream) const
{
	qDebug() << "SendConnect written";
	stream << *lobby << *player;
	return stream;
}

void SendConnect::execute()
{
	lobby->connect(player);
	qDebug() << "execute SendConnect command";
}
*/

/*SendError::SendError(QString _error)
	: error(_error)
{
}

QDataStream& SendError::operator>>(QDataStream& stream)
{
	qDebug() << "SendError read";
	stream >> error;
	return stream;
}

QDataStream& SendError::operator<<(QDataStream& stream) const
{
	qDebug() << "SendError written";
	stream << error;
	return stream;
}

void SendError::execute()
{
	qDebug() << "execute SendError command";
}*/

SendMessage::SendMessage(QString _message, bool _error)
	: message(_message)
	, errorFlag(_error)
{
	qDebug() << "SendMessage command created";
}

QDataStream& SendMessage::operator>>(QDataStream& stream)
{
	qDebug() << "SendMessage read";
	stream >> message >> errorFlag;
	return stream;
}

QDataStream& SendMessage::operator<<(QDataStream& stream) const
{
	qDebug() << "SendMessage written";
	stream << message << errorFlag;
	return stream;
}

void SendMessage::execute()
{
	qDebug() << "execute SendMessage command:" << message;
}
