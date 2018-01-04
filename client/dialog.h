#ifndef DIALOG_H
#define DIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class CreateLobbyDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CreateLobbyDialog(QWidget* parent = 0);
	~CreateLobbyDialog();

public slots:
	createLobby();

private:
	QVBoxLayout* mainLayout;
	QLineEdit* lobbyNameEdit;
	QLineEdit* hostLoginEdit;
	QComboBox* gameTypeEdit;
	QPushButton* createLobbyBtn;
};

#endif // DIALOG_H
