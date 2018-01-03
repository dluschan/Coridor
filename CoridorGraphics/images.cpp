#include "images.h"

void Images::load()
{
	images.insert(-1, QImage("PICTURES/empty_tile.png"));
	images.insert(-2, QImage("PICTURES/empty_tile2.png"));
	images.insert(0, QImage("PICTURES/player1.png"));
	images.insert(1, QImage("PICTURES/player2.png"));
	images.insert(5, QImage("PICTURES/wall_tile.png"));
}

void Images::load2()
{
	images.insert(-1, QImage("PICTURES/empty_tile2.png"));
	for (int i = 0; i < 16; i++)
		images.insert(i, QImage("PICTURES/figure" + QString::number(i) + ".png"));
	images.insert(0, QImage("PICTURES/figure0.png"));
}

QImage& Images::getImage(const int& imgNumber)
{
	QMap<int, QImage>::iterator i = images.find(imgNumber);
	if (i == images.end())
		throw 1;
	return i.value();
}
