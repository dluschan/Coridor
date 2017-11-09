#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(); // Инициализируем графическую сцену

    ui->graphicsView->setScene(scene); // Устанавливаем графическую сцену в graphicsView

    QString tmp;
    tmp = "f3_nimbus";
    qreal x = 0;
    qreal y = 0;
    scene->addItem(new Sprite(tmp, x, y, 101, 8)); // Помещаем на сцену новый объект спрайта

    scene->addRect(scene->sceneRect());
}

Widget::~Widget()
{
    delete ui;
}
