#ifndef PLAYER_H
#define PLAYER_H

#include <QDebug>
#include <QString>

class Player
{
public:
	Player(QString login = "");

	QString playerName;
	int ID, wins, loses;

	void setID(int socketDescriptor);
};

#endif // PLAYER_H
