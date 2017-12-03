#include "player.h"

Player::Player(QString login, int socketDescriptor)
    : playerName(login)
    , ID(socketDescriptor)
{
    //...
}
