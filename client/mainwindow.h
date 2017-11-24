#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
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

    QTcpSocket* pSocket;
    QByteArray data;

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void connectToTheServer();
    void slotLabelUpdate();
    void sendMessage();

signals:
    void signalLabelUpdate();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
