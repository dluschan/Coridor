#ifndef IMAGES_H
#define IMAGES_H

#include <QImage>
#include <QMap>
#include <QString>

class Images
{
public:
	void load();
	QImage& getImage(const int& imgNumber);

private:
	QMap<int, QImage> images;
};

#endif // IMAGES_H
