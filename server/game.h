#ifndef GAME_H
#define GAME_H

#include "player.h"
#include <list>

enum GameType
{
	Coridor,
	WrongType
};

class Game
{
public:
	Game(GameType type);
	GameType game;
};

#endif // GAME_H
