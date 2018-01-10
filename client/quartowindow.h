#ifndef QUARTOWINDOW_H
#define QUARTOWINDOW_H

#include "../common/quartologic.h"
#include "field.h"
#include "images.h"
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QWidget>

namespace Ui
{
	class QuartoWindow;
}

struct Figures
{
	bool figures[16];
	bool fSelected[16];
	QImage selectedFigure;
	QImage* figuresImage;
	void redrawFiguresImage(Images image);
	void update(QPoint pos);
	void mousePressed(QPoint pos);
	void mouseMoved(QPoint pos, QPoint point);
	int getSelectedID();
	int fX = 36 * 6, fY = 0, sX, sY;
};

class QuartoWindow : public QWidget
{
	Q_OBJECT

public:
	explicit QuartoWindow(QString _firstPlayer, QString _secondPlayer, QString _player, QWidget* parent = 0);
	~QuartoWindow();

protected:
	void paintEvent(QPaintEvent* pEvent);
	void mousePressEvent(QMouseEvent* mEvent);
	void mouseMoveEvent(QMouseEvent* mEvent);
	void mouseReleaseEvent(QMouseEvent* mEvent);

signals:
	void firstWindow(); // Сигнал для первого окна на открытие

	void sendQPointSignal(QPoint point, int figureId, QString enemy);
	void quartoSendCheckWinSignal(QString enemy, bool checkWin);

private slots:
	void on_pushButton_clicked();
	void on_checkWin_clicked();
	void on_start_clicked();
	void on_nextTurn_clicked();

	void quartoRecieveQPoint(QPoint point, int figureId, QString reciever);
	void quartoRecieveCheckWin(QString _enemy, bool _checkWin);

private:
	Ui::QuartoWindow* ui;
	Images* pictures;
	QImage* image;
	Field* field;
	QuartoLogic* game;
	Figures figures;
	QString status = "Place a figure";
	QString player;

	void placeFigure(QPoint point, int figureId);
	void checkWin();
	void nextTurn();
	bool checkPoint(QPoint point);
};

#endif // QUARTOWINDOW_H
