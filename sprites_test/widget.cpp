#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();       // Инициализируем графическую сцену

    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView

    scene->addItem(new Sprite());       // Помещаем на сцену новый объект спрайта

}

Widget::~Widget()
{
    delete ui;
}
