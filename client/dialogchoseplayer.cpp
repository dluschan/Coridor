#include "dialogchoseplayer.h"

DialogChosePlayer::DialogChosePlayer(bool _hosting, QString host, QString player, QWidget* parent)
	: QDialog(parent)
{
	mainLayout = new QGridLayout();
	setLayout(mainLayout);

	if (_hosting)
	{
		this->setWindowTitle("Choose first player");
		firstPlayerEdit = new QComboBox;
		firstPlayerEdit->addItem(host);
		firstPlayerEdit->addItem(player);
		chooseFirstPlayerBtn = new QPushButton("Ok");
		connect(chooseFirstPlayerBtn, SIGNAL(clicked()), this, SLOT(startTheGame()));

		mainLayout->addWidget(firstPlayerEdit);
		mainLayout->addWidget(chooseFirstPlayerBtn);
	}
	this->setMinimumHeight(40);
	this->setMinimumWidth(200);

	this->setWindowTitle("Waiting for host to shose first player");

	this->show();
}

DialogChosePlayer::~DialogChosePlayer()
{
}

void DialogChosePlayer::startTheGame()
{
	emit clicked(firstPlayerEdit->currentText());
	this->hide();
}
