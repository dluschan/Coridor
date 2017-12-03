#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QPixmap>
//#include <QTime>
#include <QTimer>
#include <QtCore>

class Sprite : public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	explicit Sprite(QObject* parent = 0);
	bool dvig, selected;

private:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	QRectF boundingRect() const;

private:
	qreal x, y, t, u, distance, mx, my;
	// QTimer* timer; // Таймер для пролистывания изображения в QPixmap
	// QTimer* timer2;
	QPixmap* spriteImage; // В данный объект QPixamp будет помещён спрайт
	int currentFrame;	 // Координата X, с которой начинает очередной кадр спрайта
	float time;

signals:
	void released(qreal i, qreal j);

public slots:
	void nextFrame(); // Слот для пролистывания изображения в QPixmap
	void move();
	void select(qreal i, qreal j);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // SPRITE_H
