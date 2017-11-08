#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(); // Инициализируем графическую сцену

    ui->graphicsView->setScene(scene); // Устанавливаем графическую сцену в graphicsView

    Sprite* sprite = new Sprite();
    scene->addItem(sprite); // Помещаем на сцену новый объект спрайта

    QPen MyPen = QPen(Qt::red);

    /*QLineF TopLine(scene->sceneRect().topLeft(), scene->sceneRect().topRight());
    QLineF RightLine(scene->sceneRect().topRight(), scene->sceneRect().bottomRight());
    QLineF BottomLine(scene->sceneRect().bottomRight(), scene->sceneRect().bottomLeft());
    QLineF LeftLine(scene->sceneRect().bottomLeft(), scene->sceneRect().topLeft());

    scene->addLine(TopLine, MyPen);
    scene->addLine(RightLine, MyPen);
    scene->addLine(BottomLine, MyPen);
    scene->addLine(LeftLine, MyPen);*/

    scene->addRect(scene->sceneRect());

    this->setMouseTracking(true);

    timer = new QTimer();
    timer2 = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), SLOT(slotUpdateCurPos()));
    QObject::connect(timer, SIGNAL(timeout()), sprite, SLOT(nextFrame()));
    QObject::connect(timer2, SIGNAL(timeout()), sprite, SLOT(move()));
    timer->start(75);
    timer2->start(0.001);

    // while (this->pollEvent)
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotUpdateCurPos()
{
    // ui->label->setText(QTime::currentTime().toString());
    // ui->label->setText(tr("x = %1, y = %2").arg(cursor().pos().x()).arg(cursor().pos().y()));
    ui->label->setText(tr("x = %1, y = %2").arg(this->mapFromGlobal(QCursor::pos()).x()).arg(this->mapFromGlobal(QCursor::pos()).y()));
    x = this->mapFromGlobal(QCursor::pos()).x();
    y = this->mapFromGlobal(QCursor::pos()).y();
}
