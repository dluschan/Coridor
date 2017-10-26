#ifndef WIDGET_H
#define WIDGET_H

#include <QGraphicsScene>
#include <QList>
//#include <QPixmap>
//#include <QTimer>
#include <QWidget>

#include "sprite.h"

namespace Ui
{
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = 0);
    ~Widget();
    qreal x, y;
    Sprite* sprite;

private:
    Ui::Widget* ui;
    QGraphicsScene* scene; // Объявляем графическую сцену
    QTimer* timer;
    QTimer* timer2;
    // QCursor::pos();

private slots:
    void slotUpdateCurPos();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // WIDGET_H
