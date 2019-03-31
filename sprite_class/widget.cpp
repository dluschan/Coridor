#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);

	// QPoint viewPos = myEvent->pos();
	// QPointF scenePos = ui->graphicsView->mapFromGlobal(viewPos);

	scene = new QGraphicsScene(); // Инициализируем графическую сцену

	ui->graphicsView->setScene(scene); // Устанавливаем графическую сцену в graphicsView

	QString tmp;
	tmp = "f3_nimbus";
	qreal x = 0;
	qreal y = 0;
	Sprite* nimbus = new Sprite(tmp, x, y, 101, 8);
	scene->addItem(nimbus); // Помещаем на сцену новый объект спрайта

	scene->addRect(scene->sceneRect());

	this->setMouseTracking(true);

	timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), SLOT(slotUpdateCurPos()));
	QObject::connect(this, SIGNAL(pos(qreal, qreal)), nimbus, SLOT(updateCurPos(qreal, qreal)));
	timer->start(75);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::slotUpdateCurPos()
{
	ui->label->setText(tr("x = %1, y = %2").arg(this->mapFromGlobal(QCursor::pos()).x()).arg(this->mapFromGlobal(QCursor::pos()).y()));
	emit pos(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());
	// x = this->mapFromGlobal(QCursor::pos()).x();
	// y = this->mapFromGlobal(QCursor::pos()).y();
}
