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

private:
    Ui::Widget* ui;
    QGraphicsScene* scene; // Объявляем графическую сцену
    QTimer* timer;

private slots:
    void slotUpdateCurPos();
};

#endif // WIDGET_H
