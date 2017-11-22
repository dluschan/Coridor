#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pSocket = new QTcpSocket(this);
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(this, SIGNAL(signalLabelUpdate()), this, SLOT(slotLabelUpdate()));
    connect(ui->Enter, SIGNAL(released()), this, SLOT(sendMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    pSocket->connectToHost("127.0.0.1", 5555);
}

void MainWindow::sockDisc()
{
    pSocket->deleteLater();
}
void MainWindow::sockReady()
{
    if (pSocket->waitForConnected(500))
    {
        pSocket->waitForReadyRead(500);
        data = pSocket->readAll();
        qDebug() << data;
        emit signalLabelUpdate();
        //        QDataStream in(ppSocket);
        //        in.setVersion(QDataStream::Qt_4_0);
        //        in >> data;
        //        qDebug() << data;
        //        emit signalLabelUpdate();
    }
}

void MainWindow::slotLabelUpdate()
{
    ui->label->setText(QString(data));
}

void MainWindow::sendMessage()
{
    // Data = QByteArray(ui->lineEdit->text());
    // pSocket->write(Data);

    /*QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) << QTime::currentTime() << ui->lineEdit->text();

    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));

    pSocket->write(arrBlock);*/

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << ui->lineEdit->text();
    pSocket->write(arrBlock);
    pSocket->waitForBytesWritten();

    ui->lineEdit->setText("");

    qDebug() << "Message Sent";
}
