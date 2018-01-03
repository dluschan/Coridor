#ifndef IMAGES_H
#define IMAGES_H

#include <QImage>
#include <QMap>
#include <QString>

class Images
{
public:
	void load();
	void load2();
	QImage& getImage(const int& imgNumber);

private:
	QMap<int, QImage> images;
};

#endif // IMAGES_H
