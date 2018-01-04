#include "lobby.h"

Lobby::Lobby(QString _name, QString _host, int _gameType)
	: lobbyName(_name)
	, host(new Player(_host))
	, game((GameType)_gameType)
{
}
