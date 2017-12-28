#ifndef FIELD_H
#define FIELD_H

#include "images.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>

class Field
{
public:
	Field(Images* images, int _left, int _top, int _width, int _height);
	~Field();
	const QImage& getImage() const;
	void redraw(int pole[17][17]);
	QPoint getCoord(int x, int y);
	int getX();
	int getY();

	// int pole[17][17];

private:
	QImage* image;
	Images* pictures;
	int left, top, width, height;
};

#endif // FIELD_H
