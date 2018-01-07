#include "lobby.h"

Lobby::Lobby(QString _name, QString _host, int _gameType, int _connectedPlayersNumber)
	: lobbyName(_name)
	, host(new Player(_host))
	, gameType((GameType)_gameType)
	, connectedPlayersNumber(_connectedPlayersNumber)
{
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

int Lobby::getGameType(QString _gameTypeStr)
{
	// qDebug() << int(gameType);
	if (_gameTypeStr == "Coridor")
		return 0;
	else if (_gameTypeStr == "Quarto")
		return 1;
	// return int(gameType);
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
