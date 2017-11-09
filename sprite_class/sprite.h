#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QtCore>

class Sprite : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Sprite(QString PN, qreal X, qreal Y, int S, int FN, QObject* parent = 0);

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QRectF boundingRect() const;

    qreal x, y, t, u, time;
    bool dvig, selected;
    QTimer* timer;						  // Таймер для пролистывания изображения в QPixmap
    int currentFrame, size, framesNumber; // Координата X, с которой начинает очередной кадр спрайта
    QPixmap* spriteImage;				  // В данный объект QPixamp будет помещён спрайт

signals:
    released(qreal i, qreal j);

public slots:
    void nextFrame(); // Слот для пролистывания изображения в QPixmap
    void move();
    void select(qreal i, qreal j);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // SPRITE_H
