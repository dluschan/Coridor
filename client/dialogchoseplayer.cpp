#include "dialogchoseplayer.h"

DialogChoosePlayer::DialogChoosePlayer(bool _hosting, QString host, QString player, QWidget* parent)
	: QDialog(parent)
{
	mainLayout = new QGridLayout();
	setLayout(mainLayout);

	this->setWindowTitle("Waiting for host to shose first player");
	this->setMinimumHeight(40);
	this->setMinimumWidth(350);

	if (_hosting)
	{
		this->setWindowTitle("Choose first player");
		this->setMinimumHeight(60);
		this->setMinimumWidth(250);

		firstPlayerEdit = new QComboBox;
		firstPlayerEdit->addItem(host);
		firstPlayerEdit->addItem(player);
		chooseFirstPlayerBtn = new QPushButton("Ok");
		connect(chooseFirstPlayerBtn, SIGNAL(clicked()), this, SLOT(startTheGame()));

		mainLayout->addWidget(firstPlayerEdit);
		mainLayout->addWidget(chooseFirstPlayerBtn);
	}

	this->show();
}

DialogChoosePlayer::~DialogChoosePlayer()
{
}

void DialogChoosePlayer::startTheGame()
{
	emit clicked(firstPlayerEdit->currentText());
	this->close();
}

void DialogChoosePlayer::closeSlot()
{
	close();
	deleteLater();
}
