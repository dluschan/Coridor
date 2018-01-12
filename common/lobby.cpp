#include "lobby.h"

Lobby::Lobby(QString _name, QString _host, int _gameType, int _connectedPlayersNumber)
	: lobbyName(_name)
	, host(new Player(_host))
	, gameType((GameType)_gameType)
	, connectedPlayersNumber(_connectedPlayersNumber)
{
	updateGameType(gameType);
	updateStatus(status);
}

void Lobby::updateGameType(int _gameType)
{
	gameType = (GameType)_gameType;
	switch (gameType)
	{
	case Coridor:
		gameTypeStr = "Coridor";
		break;
	case Quarto:
		gameTypeStr = "Quarto";
		break;
	default:
		// Wrong gameType
		break;
	}
}

void Lobby::updateStatus(int _status)
{
	status = (Status)_status;
	switch (status)
	{
	case Unready:
		statusStr = "InLobby(Unready)";
		break;
	case Ready:
		statusStr = "InLobby(Ready)";
		break;
	case InGame:
		statusStr = "InGame";
		break;
	default:
		// Wrong gameType
		break;
	}
}

void Lobby::update(int _gameType, int _status)
{
	updateGameType(_gameType);
	updateStatus(_status);
}

int Lobby::getGameType(QString _gameTypeStr)
{
	if (_gameTypeStr == "Coridor")
		return 0;
	else if (_gameTypeStr == "Quarto")
		return 1;
	return 2;
}

void Lobby::connect(Player* connectingPlayer)
{
	connectedPlayers.push_back(connectingPlayer);
	connectedPlayersNumber++;
}

void Lobby::disconnect(Player* disconnectingPlayer)
{
	if (!connectedPlayers.empty())
	{
		connectedPlayers.pop_back();
		connectedPlayersNumber--;
	}
}
