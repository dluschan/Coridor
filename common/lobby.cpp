#include "lobby.h"

Lobby::Lobby(QString _name, QString _host, int _gameType)
	: lobbyName(_name)
	, host(new Player(_host))
	, gameType((GameType)_gameType)
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

void Lobby::update(int _gameType)
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
