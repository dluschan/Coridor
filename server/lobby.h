#ifndef LOBBY_H
#define LOBBY_H

#include "game.h"
//#include <QString>
//#include "player.h"

class Lobby
{
public:
    Lobby(QString _name, Player* player);
    Player* host;
    QString lobbyName;
    std::list<Player*> connectedPlayers;
    GameType game = (GameType)1;
};

#endif // LOBBY_H
