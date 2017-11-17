#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTime>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_pushButton_clicked();
    void slotLabelUpdate();
    void sendMessage();

signals:
    void signalLabelUpdate();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
