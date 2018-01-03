#include "mainwindow.h"
#include "../common/command.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	// ui->Enter->setDefault(true);
	pSocket = new QTcpSocket(this);
	connect(pSocket, SIGNAL(readyRead()), this, SLOT(sockReady()));
	connect(pSocket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
	connect(this, SIGNAL(signalLabelUpdate()), this, SLOT(slotLabelUpdate()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(connectToTheServer()));
	// connect(ui->Enter, SIGNAL(clicked()), this, SLOT(sendMessage()));
	connect(ui->Enter, SIGNAL(clicked()), this, SLOT(sendCommand()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::connectToTheServer()
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
	}
}

void MainWindow::slotLabelUpdate()
{
	ui->label->setText(QString(data));
}

void MainWindow::sendMessage()
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);
	out << ui->lineEdit->text();
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	// ui->lineEdit->setText("");

	qDebug() << "Message Sent";
}

void MainWindow::sendCommand()
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	CommandType commandType = {0};
	Command* pCommand = new Login(ui->lineEdit->text());

	out << commandType;
	pCommand->operator<<(out);
	pSocket->write(arrBlock);
	pSocket->waitForBytesWritten();

	ui->lineEdit->setText("");

	qDebug() << "Command Sent";
}
