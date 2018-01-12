#include "command.h"
#include <QDebug>

// запись и вывод CommandType в/из поток(а)
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

// запись и вывод Player в/из поток(а)
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

// запись и вывод Lobby в/из поток(а)
QDataStream& operator>>(QDataStream& stream, Lobby& lobby)
{
	QString lobbyName;
	QString hostHame;
	unsigned int gameType;
	int status;
	int connectedPlayersNumber;
	int size;

	stream >> lobbyName >> hostHame >> gameType >> status >> connectedPlayersNumber >> size;
	lobby = Lobby(lobbyName, hostHame, gameType, connectedPlayersNumber);
	lobby.updateStatus(status);
	Player* playerTmp = new Player();
	for (int i = 0; i < size; i++)
	{
		stream >> *playerTmp;
		lobby.connectedPlayers.push_back(playerTmp);
	}
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Lobby& lobby)
{
	stream << lobby.lobbyName << lobby.host->playerName << unsigned(lobby.gameType) << unsigned(lobby.status) << lobby.connectedPlayersNumber << lobby.connectedPlayers.size();
	for (const auto& i : lobby.connectedPlayers)
		stream << *i;
	return stream;
}

// запись и вывод GameType в/из поток(а)
/*QDataStream& operator>>(QDataStream& stream, GameType& gameType)
{
	unsigned int type;
	stream >> type;
	gameType = (GameType)type;
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const GameType& gameType)
{
	stream << int(GameType);
	return stream;
}*/

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
	case CommandType::Type::UpdateLobby:
	{
		list<Player*> playersEmpty;
		pCommand = new UpdateLobby(0, 0, playersEmpty);
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
	case CommandType::Type::SendFirstPlayer:
		pCommand = new SendFirstPlayer();
		break;
	case CommandType::Type::CoridorSendQPoint:
		pCommand = new CoridorSendQPoint();
		break;
	case CommandType::Type::QuartoSendQPoint:
		pCommand = new QuartoSendQPoint();
		break;
	case CommandType::Type::QuartoSendCheckWin:
		pCommand = new QuartoSendCheckWin();
		break;
	/*case CommandType::Type::SendStart:
	{
		list<Player*> playersEmpty;
		pCommand = new SendStart(playersEmpty);
		break;
	}*/
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

UpdateLobby::UpdateLobby(int _gameType, int _status, std::list<Player*> _connectedPlayers)
	: gameType(_gameType)
	, status(_status)
	, connectedPlayers(_connectedPlayers)
{
	qDebug() << "UpdateLobby command created" << gameType << status;
	// connectedPlayers.push_back(host);
}

QDataStream& UpdateLobby::operator>>(QDataStream& stream)
{
	qDebug() << "UpdateLobby read";

	Player* playerTmp = new Player();
	int size;
	stream >> size >> gameType >> status;
	qDebug() << gameType << status;
	for (int i = 0; i < size; i++)
	{
		stream >> *playerTmp;
		connectedPlayers.push_back(playerTmp);
	}

	return stream;
}

QDataStream& UpdateLobby::operator<<(QDataStream& stream) const
{
	qDebug() << "UpdateLobby written";

	stream << connectedPlayers.size() << gameType << status;

	for (const auto& i : connectedPlayers)
	{
		stream << *i;
	}

	return stream;
}

void UpdateLobby::execute()
{
	qDebug() << "execute UpdateLobby command" << gameType;
}

DeleteLobby::DeleteLobby(Lobby* _lobby)
	: lobby(_lobby)
{
	for (const auto& i : _lobby->connectedPlayers)
		// lobby->connectedPlayers.push_back(i);
		qDebug() << i->playerName;
	qDebug() << "DeleteLobby command created" << lobby->lobbyName;
}

QDataStream& DeleteLobby::operator>>(QDataStream& stream)
{
	qDebug() << "DeleteLobby read";
	stream >> *lobby;
	return stream;
}

QDataStream& DeleteLobby::operator<<(QDataStream& stream) const
{
	qDebug() << "DeleteLobby written";
	stream << *lobby;
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
	Lobby* lobbyTmp2 = new Lobby();
	int size;
	stream >> size;
	for (int i = 0; i < size; i++)
	{
		stream >> *lobbyTmp;
		lobbyTmp2 = new Lobby(lobbyTmp->lobbyName, lobbyTmp->host->playerName, lobbyTmp->gameType, lobbyTmp->connectedPlayersNumber);
		lobbies.push_back(lobbyTmp2);
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

SendFirstPlayer::SendFirstPlayer(QString _firstPlayer, QString _guest, int _gameType)
	: firstPlayer(_firstPlayer)
	, guest(_guest)
	, gameType(_gameType)
{
	qDebug() << "SendFirstPlayer command created";
}

QDataStream& SendFirstPlayer::operator>>(QDataStream& stream)
{
	stream >> firstPlayer >> guest >> gameType;
	return stream;
}

QDataStream& SendFirstPlayer::operator<<(QDataStream& stream) const
{
	stream << firstPlayer << guest << gameType;
	return stream;
}

void SendFirstPlayer::execute()
{
	qDebug() << "execute SendFirstPlayer command";
}

CoridorSendQPoint::CoridorSendQPoint(QPoint _point, bool _move, QString _enemy, bool _horizontal)
	: point(_point)
	, move(_move)
	, enemy(_enemy)
	, horizontal(_horizontal)
{
	qDebug() << "CoridorSendQPoint command created";
}

QDataStream& CoridorSendQPoint::operator>>(QDataStream& stream)
{
	stream >> point >> move >> enemy >> horizontal;
	qDebug() << "CoridorSendQPoint read" << enemy;
	return stream;
}

QDataStream& CoridorSendQPoint::operator<<(QDataStream& stream) const
{
	stream << point << move << enemy << horizontal;
	qDebug() << "CoridorSendQPoint written" << enemy;
	return stream;
}

void CoridorSendQPoint::execute()
{
	qDebug() << "execute CoridorSendQPoint command";
}

QuartoSendQPoint::QuartoSendQPoint(QPoint _point, int _figureId, QString _enemy)
	: point(_point)
	, figureId(_figureId)
	, enemy(_enemy)
{
	qDebug() << "QuartoSendQPoint command created";
}

QDataStream& QuartoSendQPoint::operator>>(QDataStream& stream)
{
	stream >> point >> figureId >> enemy;
	qDebug() << "QuartoSendQPoint read" << enemy;
	return stream;
}

QDataStream& QuartoSendQPoint::operator<<(QDataStream& stream) const
{
	stream << point << figureId << enemy;
	qDebug() << "QuartoSendQPoint written" << enemy;
	return stream;
}

void QuartoSendQPoint::execute()
{
	qDebug() << "execute QuartoSendQPoint command";
}

QuartoSendCheckWin::QuartoSendCheckWin(QString _enemy, bool _checkWin)
	: enemy(_enemy)
	, checkWin(_checkWin)
{
	qDebug() << "QuartoSendCheckWin command created";
}

QDataStream& QuartoSendCheckWin::operator>>(QDataStream& stream)
{
	stream >> enemy >> checkWin;
	qDebug() << "QuartoSendCheckWin read" << enemy;
	return stream;
}

QDataStream& QuartoSendCheckWin::operator<<(QDataStream& stream) const
{
	stream << enemy << checkWin;
	qDebug() << "QuartoSendCheckWin written" << enemy;
	return stream;
}

void QuartoSendCheckWin::execute()
{
	qDebug() << "execute QuartoSendCheckWin command";
}

/*SendStart::SendStart(list<Player*> _players)
	: players(_players)
{
	qDebug() << "SendStart command created";
}

QDataStream& SendStart::operator>>(QDataStream& stream)
{
	qDebug() << "SendStart read";
	Player* playerTmp = new Player();
	int size;
	stream >> size;
	for (int i = 0; i < size; i++)
	{
		stream >> *playerTmp;
		players.push_back(playerTmp);
	}
	return stream;
}

QDataStream& SendStart::operator<<(QDataStream& stream) const
{
	qDebug() << "SendStart written";
	stream << players.size();
	for (const auto& i : players)
	{
		stream << *i;
	}
	return stream;
}

void SendStart::execute()
{
	qDebug() << "execute SendStart command";
}*/

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
