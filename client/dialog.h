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

	QPushButton* createLobbyBtn;
	QLineEdit* lobbyNameEdit;
	QLineEdit* hostLoginEdit;
	QComboBox* gameTypeEdit;

public slots:
	createLobby();

signals:
	clicked2(QString ln, QString hl, int gt);

private:
	QVBoxLayout* mainLayout;
};

#endif // DIALOG_H
