#ifndef LOBBY_H
#define LOBBY_H

#include "player.h"
#include <QObject>

enum GameType
{
	Coridor,
	Quarto,
	WrongGameType
};

enum Status
{
	Unready,
	Ready,
	InGame
};

class Lobby
{
public:
	Lobby(QString _name = "LobbyName", QString _host = "HostName", int _gameType = 2, int _connectedPlayersNumber = 1);
	QString lobbyName;
	Player* host;
	int connectedPlayersNumber = 1, maxPlayers = 2;
	Status status = Unready;
	std::list<Player*> connectedPlayers;
	GameType gameType = WrongGameType;
	QString gameTypeStr = "WrongGameType", statusStr = "Status";

	void updateGameType(int _gameType);
	void updateStatus(int _status);
	void update(int _gameType, int _status);
	int getGameType(QString _gameTypeStr);
	void connect(Player* connectingPlayer);
	void disconnect(Player* disconnectingPlayer);
};

#endif // LOBBY_H
