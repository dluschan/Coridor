#ifndef DIALOGCHOSEPLAYER_H
#define DIALOGCHOSEPLAYER_H

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

class DialogChoosePlayer : public QDialog
{
	Q_OBJECT

public:
	explicit DialogChoosePlayer(bool _hosting, QString host = "Host", QString player = "guest", QWidget* parent = 0);
	~DialogChoosePlayer();

public slots:
	void startTheGame();
	void closeSlot();

signals:
	void clicked(QString firstPl);

private:
	QGridLayout* mainLayout;

	QPushButton* chooseFirstPlayerBtn;
	QComboBox* firstPlayerEdit;
};

#endif // DIALOGCHOSEPLAYER_H
