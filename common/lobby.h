#ifndef LOBBY_H
#define LOBBY_H

#include "game.h"
#include <QObject>
//#include <QString>
//#include "player.h"

enum Status
{
	WaitingForPlayersToConnect,
	WaitingForPlayersToBeRdy,
	InGame
};

class Lobby
{
public:
	Lobby(QString _name = "LobbyName", QString _host = "HostName", int _gameType = 1);
	QString lobbyName;
	Player* host;
	int connectedPlayersNumber = 1, maxPlayers = 2, status = WaitingForPlayersToConnect;
	std::list<Player*> connectedPlayers;
	GameType gameType;
	QString gameTypeStr;

	void update(int _gameType);
};

#endif // LOBBY_H
