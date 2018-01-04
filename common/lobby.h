#ifndef LOBBY_H
#define LOBBY_H

#include "game.h"
//#include <QString>
//#include "player.h"

class Lobby
{
public:
	Lobby(QString _name, QString _host, int _gameType);
	QString lobbyName;
	Player* host;
	std::list<Player*> connectedPlayers;
	GameType game = (GameType)1;
};

#endif // LOBBY_H
