#include "quartowindow.h"
#include "ui_quartowindow.h"

void Figures::redrawFiguresImage(Images pictures)
{
	figuresImage->fill(0);
	QPainter painter(figuresImage);
	double cfx = 1.0 * 36 * 5.0 / 4.0;
	double cfy = 1.0 * 36 * 5.0 / 4.0;
	for (int i = 0; i < 16; i++)
	{
		if (figures[i] && !fSelected[i])
			painter.drawImage(i % 4 * cfx, i / 4 * cfy, pictures.getImage(i));
	}
}

int Figures::getSelectedID()
{
	for (int i = 0; i < 16; i++)
	{
		if (fSelected[i])
		{
			return i;
		}
	}
	return -1;
}

void Figures::update(QPoint pos)
{
	selectedFigure = QImage("PICTURES/figure" + QString::number(getSelectedID()) + ".png");
	sX = pos.x() - 18;
	sY = pos.y() - 18;
}

void Figures::mousePressed(QPoint pos)
{
	if (pos.x() >= fX && pos.x() <= fX + 36 * 5 && pos.y() >= fY && pos.y() <= fY + 36 * 5)
	{
		double cfx = 1.0 * 36 * 5.0 / 4.0;
		double cfy = 1.0 * 36 * 5.0 / 4.0;

		if (figures[int((pos.x() - fX) / cfx) % 4 + int((pos.y() - fY) / cfy) % 4 * 4])
		{
			fSelected[int((pos.x() - fX) / cfx) % 4 + int((pos.y() - fY) / cfy) % 4 * 4] = true;
			figures[int((pos.x() - fX) / cfx) % 4 + int((pos.y() - fY) / cfy) % 4 * 4] = false;
		}
	}
	update(pos);
}

void Figures::mouseMoved(QPoint pos, QPoint point)
{
	if (point.x() >= 0 && point.x() <= 4 && point.y() >= 0 && point.y() <= 4)
	{
		double cfx = 1.0 * 36 * 5 / 4.0;
		double cfy = 1.0 * 36 * 5 / 4.0;
		pos.setX(point.x() * cfx + 18);
		pos.setY(point.y() * cfy + 18);
	}
	update(pos);
}

QuartoWindow::QuartoWindow(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::QuartoWindow)
{
	ui->setupUi(this);
	// connect(ui->pushButton_2, SIGNAL(pressed()), this, SLOT(start_pushButton_clicked()));

	pictures = new Images;
	pictures->load2();

	field = new Field(pictures, 0, 0, 36 * 5, 36 * 5);

	for (int i = 0; i < 16; i++)
	{
		figures.figures[i] = true;
		figures.fSelected[i] = false;
	}
	figures.figuresImage = new QImage(36 * 5, 36 * 5, QImage::Format_ARGB32);

	game = new QuartoLogic();

	figures.redrawFiguresImage(*pictures);
	field->redraw2(game->pole);
}

QuartoWindow::~QuartoWindow()
{
	delete ui;
}

void QuartoWindow::paintEvent(QPaintEvent* pEvent)
{
	QPainter painter(this);
	field->redraw2(game->pole);
	if (!game->end)
		if (game->turnIsDone)
			status = "End the turn";
		else
			status = "Place a figure";
	ui->status->setText(status);
	ui->turn->setText("Turn of player " + QString::number(game->player));
	painter.drawImage(field->getX(), field->getY(), field->getImage());
	figures.redrawFiguresImage(*pictures);
	painter.drawImage(figures.fX, figures.fY, *figures.figuresImage);
	if (figures.getSelectedID() != -1)
		painter.drawImage(figures.sX, figures.sY, figures.selectedFigure);
}

void QuartoWindow::mousePressEvent(QMouseEvent* mEvent)
{
	QPoint pos = mEvent->pos();
	if (!game->end)
		figures.mousePressed(pos);

	this->update();
}

void QuartoWindow::mouseMoveEvent(QMouseEvent* mEvent)
{
	QPoint pos = mEvent->pos();
	QPoint point = field->getCoord2(pos.x(), pos.y());
	figures.mouseMoved(pos, point);

	this->update();
}

void QuartoWindow::mouseReleaseEvent(QMouseEvent* mEvent)
{
	QPoint pos = mEvent->pos();
	QPoint point = field->getCoord2(pos.x(), pos.y());

	if (figures.getSelectedID() != -1)
	{
		if (game->placeFigure(point.x(), point.y(), figures.getSelectedID()))
			figures.figures[figures.getSelectedID()] = true;
		figures.fSelected[figures.getSelectedID()] = false;
	}

	this->update();
}

void QuartoWindow::on_pushButton_clicked()
{
	this->close();		// Закрываем окно
	emit firstWindow(); // И вызываем сигнал на открытие главного окна
}

void QuartoWindow::on_checkWin_clicked()
{
	game->checkWin();
	status = "Player" + QString::number(game->winner) + " wins!";

	this->update();
}

void QuartoWindow::on_start_clicked()
{
	field = new Field(pictures, 0, 0, 36 * 5, 36 * 5);

	for (int i = 0; i < 16; i++)
	{
		figures.figures[i] = true;
		figures.fSelected[i] = false;
	}

	game = new QuartoLogic();

	this->update();
}

void QuartoWindow::on_nextTurn_clicked()
{
	game->nextTurn();
}
