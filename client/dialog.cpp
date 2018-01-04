#include "dialog.h"

CreateLobbyDialog::CreateLobbyDialog(QWidget* parent)
	: QDialog(parent)
{
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	lobbyNameEdit = new QLineEdit();
	hostLoginEdit = new QLineEdit();
	gameTypeEdit = new QComboBox;
	gameTypeEdit->addItem(tr("Coridor"), 1);
	gameTypeEdit->addItem(tr("Quarto"), 2);
	createLobbyBtn = new QPushButton("Create lobby");
	connect(createLobbyBtn, SIGNAL(clicked()), this, SLOT(createLobby()));

	mainLayout->addWidget(lobbyNameEdit);
	mainLayout->addWidget(hostLoginEdit);
	mainLayout->addWidget(gameTypeEdit);

	this->show();
}

CreateLobbyDialog::~CreateLobbyDialog()
{
	//
}

CreateLobbyDialog::createLobby()
{
	this->hide();
}
