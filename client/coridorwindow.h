#ifndef CORIDORWINDOW_H
#define CORIDORWINDOW_H

#include "../common/command.h"
#include "coridorlogic.h"
#include "dialogchoseplayer.h"
#include "field.h"
#include "images.h"
#include <QObject>
#include <QPainter>
#include <QWidget>

namespace Ui
{
	class CoridorWindow;
}

struct Walls
{
	QImage gorWall = QImage("PICTURES/GorWall.png");
	QImage verWall = QImage("PICTURES/VerWall.png");
	int hX = 662, hY = 100, vX = 700, vY = 150;
	bool hSelected = false, vSelected = false;
	void update(QPoint pos);
	void mousePressed(QPoint pos);
	void mouseMoved(QPoint pos, QPoint point);
	void mouseReleased(QPoint pos);
};

class CoridorWindow : public QWidget
{
	Q_OBJECT

public:
	explicit CoridorWindow(QString _firstPlayer, QString _secondPlayer, QString _player, QWidget* parent = 0);
	~CoridorWindow();

protected:
	void paintEvent(QPaintEvent* pEvent);
	void mousePressEvent(QMouseEvent* mEvent);
	void mouseMoveEvent(QMouseEvent* mEvent);
	void mouseReleaseEvent(QMouseEvent* mEvent);

signals:
	void firstWindow(); // Сигнал для первого окна на открытие
	void coridorSendQPointSignal(QPoint point, bool move, QString enemy, bool horizontal);
	void quartoSendQPointSignal(QPoint point, int figureId, QString enemy);

private slots:
	// Слот-обработчик нажатия кнопки
	void exitBtn_clicked();
	void start_pushButton_clicked();

	void coridorRecieveQPoint(QPoint point, bool move, QString reciever, bool horizontal);

private:
	Ui::CoridorWindow* ui;
	Images* pictures;
	QImage* image;
	Field* field;
	Walls walls;
	CoridorLogic* game;
	QString status = "Move or place a wall";
	QString player;

	void placeWall(QPoint point, bool horizontal);
	void movePlayer(QPoint point);
	bool checkPoint(QPoint point);
	// void nextTurn(int res);
	// void end();
};

#endif // CORIDORWINDOW_H
