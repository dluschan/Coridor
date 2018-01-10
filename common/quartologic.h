#ifndef QUARTOLOGIC_H
#define QUARTOLOGIC_H

#include <QString>
#include <assert.h>
#include <exception>
#include <iostream>
#include <map>
#include <queue>

struct QuartoPlayer
{
	QString name;
	int id;
};

class QuartoLogic
{
public:
	QuartoLogic(QString firstPlayer, QString secondPlayer);

	int pole[4][4];
	int currentPlayerId = 0, winner;
	bool end = false;
	bool turnIsDone = false;

	bool placeFigure(int x, int y, int figureId);
	void checkWin();
	void nextTurn();

	QuartoPlayer player1, player2;

	QString getPlayerName(int currentId);
	bool checkTurn(QString playerName);
};

#endif // QUARTOLOGIC_H
