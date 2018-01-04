#include "lobby.h"

Lobby::Lobby(QString _name, Player* player, int _lobbyType)
	: lobbyName(_name)
	, host(player)
	, game((GameType)_lobbyType)
{
	//...
}
