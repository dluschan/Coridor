#ifndef DIALOGCHOSEPLAYER_H
#define DIALOGCHOSEPLAYER_H

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

class DialogChosePlayer : public QDialog
{
	Q_OBJECT

public:
	explicit DialogChosePlayer(bool _hosting, QString host = "Host", QString player = "guest", QWidget* parent = 0);
	~DialogChosePlayer();

public slots:
	void startTheGame();

signals:
	void clicked(QString firstPl);

private:
	QGridLayout* mainLayout;

	QPushButton* chooseFirstPlayerBtn;
	QComboBox* firstPlayerEdit;
};

#endif // DIALOGCHOSEPLAYER_H
