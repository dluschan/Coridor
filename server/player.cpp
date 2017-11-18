#include "player.h"

Player::Player(QString login, int socketDescriptor)
    : name(login)
    , ID(socketDescriptor)
{
    //...
}
