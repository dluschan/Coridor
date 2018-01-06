#ifndef LOBBY_H
#define LOBBY_H

#include "game.h"
//#include <QString>
//#include "player.h"

class Lobby
{
public:
	Lobby(QString _name = "LobbyName", QString _host = "HostName", int _gameType = 1);
	QString lobbyName;
	Player* host;
	int connectedPlayersNumber = 1, maxPlayers = 2, status = 0;
	std::list<Player*> connectedPlayers;
	GameType gameType;
	QString gameTypeStr;
};

#endif // LOBBY_H
