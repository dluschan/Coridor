#ifndef QUARTOLOGIC_H
#define QUARTOLOGIC_H

#include <assert.h>
#include <exception>
#include <iostream>
#include <map>
#include <queue>

class QuartoLogic
{
public:
	QuartoLogic();

	int pole[4][4];
	int player = 1, winner;
	bool end = false;
	bool turnIsDone = false;

	bool placeFigure(int x, int y, int figureId);
	void checkWin();
	void nextTurn();
};

#endif // QUARTOLOGIC_H
