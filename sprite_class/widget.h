#ifndef WIDGET_H
#define WIDGET_H

#include "sprite.h"
#include <QGraphicsScene>
#include <QList>
#include <QWidget>

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
    // Sprite* sprite;

private:
    Ui::Widget* ui;
    QGraphicsScene* scene;
};

#endif // WIDGET_H
