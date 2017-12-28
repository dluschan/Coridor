#ifndef PLAYER_H
#define PLAYER_H

#include <assert.h>
#include <exception>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

class GameLogic
{
public:
	GameLogic();
	const int size = 9;
	const int realSize = 2 * size - 1;
	const int w = 5;

	int pole[17][17];

	int** coords;

	int end = 0;
	int player = 0; // 0 - "первый", 1 - "второй"
	int winner = 0; // 0 - "первый", 1 - "второй"

	void output(int pole[17][17]);
	bool inPole(int x, int y);
	pair<int, int> inPole(pair<int, int> p);
	pair<int, int> step(int x, int y, char ch);
	bool isPlayer(int new_x, int new_y);
	int move(int pole[17][17], int player_id, int new_x, int new_y);
	int placeWall(int pole[17][17], int player_id, int x, int y, char dir);
	/*int turn(int pole[17][17], int player_id);
	bool different_dir(char a, char b);
	void jump(int& x, int& y, int pole[17][17], char ans);*/
	bool isWall(int x0, int x1, int y0, int y1);
	bool aZvezda(int x, int y, int p);
	bool checkDistanse(int x, int new_y, int player_id);
	bool jumpCheck(int new_x, int new_y, int player_id);
};

#endif // PLAYER_H
