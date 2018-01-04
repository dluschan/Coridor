#include "dialog.h"
#include <QDebug>

CreateLobbyDialog::CreateLobbyDialog(QWidget* parent)
	: QDialog(parent)
{
	this->setWindowTitle("Create lobby");
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
	mainLayout->addWidget(createLobbyBtn);

	this->show();
}

CreateLobbyDialog::~CreateLobbyDialog()
{
	// Nothing
}

CreateLobbyDialog::createLobby()
{
	emit clicked2(lobbyNameEdit->text(), hostLoginEdit->text(), gameTypeEdit->currentIndex());
	this->hide();
}
