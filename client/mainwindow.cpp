#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(this, SIGNAL(signalLabelUpdate()), this, SLOT(slotLabelUpdate()));
    connect(ui->Enter, SIGNAL(released()), this, SLOT(sendMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1", 5555);
}

void MainWindow::sockDisc()
{
    socket->deleteLater();
}
void MainWindow::sockReady()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        qDebug() << Data;
        emit signalLabelUpdate();
    }
}

void MainWindow::slotLabelUpdate()
{
    ui->label->setText(QString(Data));
}

void MainWindow::sendMessage()
{
    //Data = QByteArray(ui->lineEdit->text());
    //socket->write(Data);

    /*QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) << QTime::currentTime() << ui->lineEdit->text();

    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));

    socket->write(arrBlock);*/

    QByteArray  arrBlock;
    QString message = ui->lineEdit->text();
    message.toLocal8Bit();
    arrBlock+=message;
    socket->write(arrBlock);
    socket->waitForBytesWritten();

    ui->lineEdit->setText("");

    qDebug() << "Message Sent";
}
