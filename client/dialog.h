#ifndef DIALOG_H
#define DIALOG_H

#include "../common/command.h"
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
	explicit CreateLobbyDialog(QString _host = QString(), QWidget* parent = 0);
	~CreateLobbyDialog();

public slots:
	void createLobby();
	void closeSlot();

signals:
	void clicked(QString ln, QString hl, int gt);

private:
	QPushButton* createLobbyBtn;
	QLineEdit* lobbyNameEdit;
	QComboBox* gameTypeEdit;

	QString host;
	QVBoxLayout* mainLayout;
};

#endif // DIALOG_H
