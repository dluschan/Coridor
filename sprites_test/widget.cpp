#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(); // Инициализируем графическую сцену

    ui->graphicsView->setScene(scene); // Устанавливаем графическую сцену в graphicsView

    scene->addItem(new Sprite()); // Помещаем на сцену новый объект спрайта

    QPen MyPen = QPen(Qt::red);

    QLineF TopLine(scene->sceneRect().topLeft(), scene->sceneRect().topRight());
    QLineF RightLine(scene->sceneRect().topRight(), scene->sceneRect().bottomRight());
    QLineF BottomLine(scene->sceneRect().bottomRight(), scene->sceneRect().bottomLeft());
    QLineF LeftLine(scene->sceneRect().bottomLeft(), scene->sceneRect().topLeft());

    scene->addLine(TopLine, MyPen);
    scene->addLine(RightLine, MyPen);
    scene->addLine(BottomLine, MyPen);
    scene->addLine(LeftLine, MyPen);

    timer = new QTimer();
    timer->start(100);
    QObject::connect(timer, SIGNAL(timeout()), SLOT(slotUpdateCurPos()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotUpdateCurPos()
{
    // ui->label->setText(QTime::currentTime().toString());
    ui->label->setText(tr("x = %1, y = %2").arg(cursor().pos().x()).arg(cursor().pos().y()));
}
