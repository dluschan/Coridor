#include "player.h"

Player::Player(QString login)
	: playerName(login)
{
	//...
}

void Player::setID(int socketDescriptor)
{
	ID = socketDescriptor;
}
