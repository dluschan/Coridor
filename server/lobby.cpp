#include "lobby.h"

Lobby::Lobby(QString _name, Player* player)
    : lobbyName(_name)
    , host(player)
{
    //...
}
