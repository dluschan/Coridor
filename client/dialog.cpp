#include "dialog.h"
#include <QDebug>

CreateLobbyDialog::CreateLobbyDialog(QString _host, QWidget* parent)
	: QDialog(parent)
	, host(_host)
{
	this->setWindowTitle("Create lobby");
	this->setModal(true);
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	lobbyNameEdit = new QLineEdit();
	gameTypeEdit = new QComboBox;
	gameTypeEdit->addItem(tr("Coridor"));
	gameTypeEdit->addItem(tr("Quarto"));
	createLobbyBtn = new QPushButton("Create lobby");
	connect(createLobbyBtn, SIGNAL(clicked()), this, SLOT(createLobby()));

	mainLayout->addWidget(lobbyNameEdit);
	mainLayout->addWidget(gameTypeEdit);
	mainLayout->addWidget(createLobbyBtn);

	this->show();
}

CreateLobbyDialog::~CreateLobbyDialog()
{
	// Nothing
}

void CreateLobbyDialog::createLobby()
{
	emit clicked(lobbyNameEdit->text(), host, gameTypeEdit->currentIndex());
	this->close();
}

void CreateLobbyDialog::closeSlot()
{
	close();
	deleteLater();
}
