#include "quartologic.h"
#include <QDebug>

QuartoLogic::QuartoLogic()
{
	for (int i = 0; i < 4; i++) //Заполняем -1
	{
		for (int j = 0; j < 4; j++)
		{
			pole[i][j] = -1;
		}
	}
}

bool QuartoLogic::placeFigure(int x, int y, int figureId)
{
	if (x >= 0 && x <= 4 && y >= 0 && y <= 4 && pole[x][y] == -1 && !turnIsDone)
	{
		pole[x][y] = figureId;
		turnIsDone = true;
		return false;
	}
	return true;
}

void QuartoLogic::checkWin()
{
	for (int i = 0; i < 4; i++)
	{
		if ((pole[i][0] & pole[i][1] & pole[i][2] & pole[i][3]) | (~pole[i][0] & ~pole[i][1] & ~pole[i][2] & ~pole[i][3]) != 0 && pole[i][0] != -1 && pole[i][1] != -1 && pole[i][2] != -1 && pole[i][3] != -1)
		{
			// qDebug() << "YES1";
			winner = player;
			end = true;
		}
		if ((pole[0][i] & pole[1][i] & pole[2][i] & pole[3][i]) | (~pole[0][i] & ~pole[1][i] & ~pole[2][i] & ~pole[3][i]) != 0 && pole[0][i] != -1 && pole[1][i] != -1 && pole[2][i] != -1 && pole[3][i] != -1)
		{
			// qDebug() << "YES2";
			winner = player;
			end = true;
		}
		if ((pole[0][0] & pole[1][1] & pole[2][2] & pole[3][3]) | (~pole[0][0] & ~pole[1][1] & ~pole[2][2] & ~pole[3][3]) != 0 && pole[0][0] != -1 && pole[1][1] != -1 && pole[2][2] != -1 && pole[3][3] != -1)
		{
			// qDebug() << "YES3";
			winner = player;
			end = true;
		}
		if ((pole[3][0] & pole[2][1] & pole[1][2] & pole[0][3]) | (~pole[3][0] & ~pole[2][1] & ~pole[1][2] & ~pole[0][3]) != 0 && pole[3][0] != -1 && pole[2][1] != -1 && pole[1][2] != -1 && pole[0][3] != -1)
		{
			// qDebug() << "YES4";
			winner = player;
			end = true;
		}
	}
	if (!end)
	{
		winner = player % 2 + 1;
		end = true;
	}
}

void QuartoLogic::nextTurn()
{
	if (turnIsDone && !end)
	{
		player = player % 2 + 1;
		turnIsDone = false;
	}
}
