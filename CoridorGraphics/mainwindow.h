#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <coridorwindow.h>
#include <quartowindow.h>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private slots:
	// Слоты от кнопок главного окна
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

private:
	Ui::MainWindow* ui;
	// второе и третье окна
	CoridorWindow* sWindow;
	QuartoWindow* tWindow;
};

#endif // MAINWINDOW_H
