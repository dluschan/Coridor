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
	Lobby(QString _name = "LobbyName", QString _host = "HostName", int _gameType = 1, int _connectedPlayersNumber = 1);
	QString lobbyName;
	Player* host;
	int connectedPlayersNumber = 1, maxPlayers = 2, status = WaitingForPlayersToConnect;
	std::list<Player*> connectedPlayers;
	GameType gameType;
	QString gameTypeStr;

	void updateGameType(int _gameType);
	int getGameType(QString _gameTypeStr);
	void connect(Player* connectingPlayer);
};

#endif // LOBBY_H
