#include "field.h"

// 57:49

Field::Field(Images* images, int _left, int _top, int _width, int _height)
	: pictures(images)
	, left(_left)
	, top(_top)
	, width(_width)
	, height(_height)
{
	/*for (int i = 0; i < 17; i++) //Заполняем -1
		for (int j = 0; j < 17; j++)
		{
			if (j % 2 == 1 || i % 2 == 1)
				pole[i][j] = -2;
			else
				pole[i][j] = -1;
		}
	pole[0][8] = 0;
	pole[16][8] = 1;*/
	image = new QImage(width, height, QImage::Format_ARGB32);
}

Field::~Field()
{
	delete image;
}

const QImage& Field::getImage() const
{
	return *image;
}

void Field::redraw(int pole[17][17])
{
	image->fill(0);
	QPainter painter(image);
	double cfx = 1.0 * width / 17.0;
	double cfy = 1.0 * height / 17.0;
	for (int i = 0; i < 17; i++)
		for (int j = 0; j < 17; j++)
		{
			painter.drawImage(i * cfx, j * cfy, pictures->getImage(pole[i][j]));
		}
	// 57:50
}

QPoint Field::getCoord(int x, int y)
{
	QPoint res;
	res.setX(-1);
	res.setY(-1);
	if (x < left || x > (left + width) || y < top || y > (top + height))
		return res;
	double cfx = 1.0 * width / 17.0;
	double cfy = 1.0 * height / 17.0;
	res.setX(1.0 * (x - left) / cfx);
	res.setY(1.0 * (y - top) / cfy);
	return res;
}

int Field::getX()
{
	return left;
}

int Field::getY()
{
	return top;
}
